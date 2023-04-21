//---------------------------------------------------------
// DEPLIAGE D'UN VOLUME version C++
// 1°) Lecture du fichier (format OBJ) contenant le volume
// 2°) Extraction des points (FAIT) et des faces (FAIT)
// 3°) Création des triangles 3d et 2d (FAIT)
// 4°) Calcul du voisinage de chaque face (FAIT)
// 5°) Calcul des coplanéités (FAIT)
// 6°) Numérotation des arêtes (FAIT)
// 7°) Creation des classes Donnees et Depliage
//---------------------------------------------------------
// Gilbert Duval 2023-03-29

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <cmath>
#include "../svg.hpp"
// https://raw.githubusercontent.com/vincentlaucsb/svg/master/src/svg.hpp

using std::cout;
using std::string;
using std::min;
using std::max;
using std::vector;
using std::array;
using std::endl;
using std::stringstream;
using std::fstream;
using std::ostream;

const double epsilon = 0.5;
const double dMin = -999999;
const double dMax =  999999;

class Vec2 {
	public:
	double x, y;

	Vec2() {}
	Vec2(double x, double y) : x(x), y(y) {}

	double distance (Vec2& v) {
		Vec2 d = Vec2 (v.x - this->x, v.y - this->y);
		return sqrt ((d.x * d.x) + (d.y * d.y));
	}

	Vec2 operator+ (const Vec2& v) {
		return Vec2 (this->x + v.x, this->y + v.y);
	}

	Vec2 operator- (const Vec2& v) {
		return Vec2 (this->x - v.x, this->y - v.y);
	}

	Vec2 operator * (const double& d) {
		return Vec2 (this->x * d, this->y * d);
	}

	Vec2 operator / (const double& d) {
		return Vec2 (this->x / d, this->y / d);
	}
};

ostream& operator << (ostream& os, const Vec2& v) {
	os << v.x << ", " << v.y;
	return os;
}

class Vec3 {
	public:
	double x, y, z;

	Vec3() {}
	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

	double distance (Vec3& v) {
		Vec3 d = Vec3 (v.x - this->x, v.y - this->y, v.z - this->z);
		return sqrt ((d.x * d.x) + (d.y * d.y) + (d.z * d.z));
	}

	Vec3 operator+(const Vec3& v) {
		return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
	}

	Vec3 operator-(const Vec3& v) {
		return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
	}

	Vec3 operator * (const double& d) {
		return Vec3 (this->x * d, this->y * d, this->z * d);
	}

	Vec3 operator / (const double& d) {
		return Vec3 (this->x / d, this->y / d, this->z / d);
	}
};

ostream& operator << (ostream& os, const Vec3& v) {
	os << v.x << ", " << v.y << ", " << v.z;
	return os;
}

class Triangle2d {
	public:
		Vec2 a, b, c;

		Triangle2d() {}
		Triangle2d(Vec2 a, Vec2 b, Vec2 c) : a(a), b(b), c(c) {}

		Vec2 point(int n) {
			return n == 0 ? this->a : n == 1 ? this->b : this->c;
		}
};

ostream& operator << (ostream& os, const Triangle2d& t) {
	os << "("<< t.a << "), (" << t.b << "), (" << t.c << ")";
	return os;
}

class Triangle3d {
	public:
		Vec3 a, b, c;

		Triangle3d() {}

		Triangle3d(Vec3 a, Vec3 b, Vec3 c) : a(a), b(b), c(c) {}

		Vec3 point(int n) {
			return n == 0 ? this->a : n == 1 ? this->b : this->c;
		}

		Triangle2d d2ize () {
			Vec3 d1 = this->b - this->a;
			Vec3 d2 = this->c - this->a;

			Vec2 P0 (0, 0);

			Vec2 P1 (
				sqrt((d1.x * d1.x) + (d1.y * d1.y) + (d1.z * d1.z)), 0);
			double P2x = ((d1.x * d2.x) + (d1.y * d2.y) + (d1.z * d2.z)) / P1.x;
			Vec2 P2 (
				P2x,
				sqrt((d2.x * d2.x) + (d2.y * d2.y) + (d2.z * d2.z) - (P2x * P2x))
			);

			return Triangle2d(P0, P1, P2);
		}

		bool eq3 (Triangle3d t2, int n) {
			Vec3 pt = t2.point(n);

			bool r = pt.distance(this->a) >= epsilon
						&& pt.distance(this->b) >= epsilon
						&& pt.distance(this->c) >= epsilon;

			return r;
		}

		double isCoplanar (Vec3 p) {
			Vec3 v1 = this->b - this->a;
			Vec3 v2 = this->c - this->a;

			double a = v1.y * v2.z - v2.y * v1.z;
			double b = v2.x * v1.z - v1.x * v2.z;
			double c = v1.x * v2.y - v1.y * v2.x;
			double d = -(a * this->a.x) 
								 -(b * this->a.y)
								 -(c * this->a.z);

			return (a * p.x) + (b * p.y) + (c * p.z) + d;
		}
};

ostream& operator << (ostream& os, const Triangle3d& t) {
	os << "("<< t.a << "), (" << t.b << "), (" << t.c << ")";
	return os;
}

class Voisin { // VOISINAGE
	public :
		int nF;
		int idx;
		double cop;

		Voisin () {}
		Voisin (int nF, int idx) : nF(nF), idx(idx) {}
};

class Copl { // COPLANEITE
	public :
		int nF;
		int nV;
		double cop;

	Copl () {}
	Copl (int nF, int nV, double cop) : nF(nF), nV(nV), cop(cop) {}
};

class Arete { // ARETE (pour numérotation)
	public :
		int nMin;
		int nMax;
		int nA;

		Arete() {}

		Arete(int n1, int n2) {
			nMin = min(n1, n2);
			nMax = max(n1, n2);
			nA = -1;
		}

		Arete(int n1, int n2, int n) {
			nMin = min(n1, n2);
			nMax = max(n1, n2);
			nA = n;
		}

		bool operator == (const Arete &a) {
			if (nMin == a.nMin && nMax == a.nMax)
				return true;
			return false;
		}
};

class Facette { // Facette à déplier
	public :
		int id;
		Triangle2d triangle;
		int groupe;
		bool pose;

		Facette() {}
		Facette(int pid, Triangle2d pt, int pg) 
			: id(pid), triangle(pt), groupe(pg), pose(false) {}
};

class Piece {
	public :
		int id;
		Vec2 pMin;
		Vec2 pMax;
		vector<Facette> facettes;
		
		Piece() {}
		Piece(int pid) : 
			id(pid), pMin(Vec2(dMax, dMax)), pMax(Vec2(dMin, dMin))
			{}
			
		void recadre(Triangle2d t) {
			for (int i = 0; i < 3; i++) {
				if (t.point(i).x < pMin.x) pMin.x = t.point(i).x;
				if (t.point(i).y < pMin.y) pMin.y = t.point(i).y;
				if (t.point(i).x > pMax.x) pMax.x = t.point(i).x;
				if (t.point(i).y > pMax.y) pMax.y = t.point(i).y;
			}
		}

		void ajouteFace(Facette f) {
			facettes.push_back(Facette(f));
			recadre(f.triangle);
		}
};

class Page {
	public :
		int id;
		vector<Piece> pieces;

		Page() {}
		Page(int pid) : id(pid) {}
		void ajoutePiece(Piece p) {
			pieces.push_back(p);
		}
};

int prec (int n) { // retourne le précédent dans le triplet (0,1,2)
	return n > 0 ? n - 1 : 2;
}

int suiv (int n) {  // retourne le suivant dans le triplet (0,1,2)
	return n < 2 ? n + 1 : 0;
}

class Donnees { // DONNEES DEPLIAGE
	private :
		vector<Vec3> pts;
		vector<vector<int>> faces;
		vector<string> groupes;
		vector<Triangle3d> t3d;
		vector<Triangle2d> t2d;
		vector<array<Voisin, 3>> voisins;
		vector<Arete> aretes;
		vector<Copl> copl;
		int nbFaces;
		vector<Facette> facettes;
		vector<Page> pages;
		string fichierOBJ;
		vector<string> vLignes;

		Facette* premFaceLibre(int g) {
			for (auto&& f : facettes) {
				if (f.groupe == g && !f.pose)
					return &f;
			}
			return nullptr;
		}

		void init_triangles() {
			for (int i = 0; i < nbFaces; i++) {
				Triangle3d t3di = Triangle3d(
					pts[faces[i][0]], 
					pts[faces[i][1]], 
					pts[faces[i][2]]);
				t3d.push_back(t3di);
				Triangle2d t2di = t3di.d2ize();
				t2d.push_back(t2di);
			}
		}

		void calc_voisinage() {
			array<Voisin, 3> tmpV;
			int vi;
			bool ok;
			for (int i = 0; i < nbFaces; i++) {
				for (int j = 0; j < 3; j++) {
					vi = 0;
					ok = false;
					do {
						if (vi != i) {
							for (int k = 0; (k < 3) && !ok; k++)
								if ((faces[vi][k] == faces[i][suiv(j)])
								 && (faces[vi][suiv(k)] == faces[i][j])) {
									tmpV[j] = Voisin(vi, suiv(k));
									if (j == 2)
										voisins.push_back(tmpV);
									ok = true;
								}
							if (!ok)
								vi++;
						} else
							vi++;
					} while ((vi < nbFaces) && !ok);
				}
			}
		}

		void calc_copl() {
			int p;
			for (int i = 0; i < nbFaces; i++) {
				for (int j = 0; j < 3; j++) {
					int nV = voisins[i][j].nF;
							 if (t3d[i].eq3(t3d[nV], 0))
						p = 0;
					else if (t3d[i].eq3(t3d[nV], 1))
						p = 1;
					else
						p = 2;
					
					copl.push_back(Copl(i, nV, t3d[i].isCoplanar(t3d[nV].point(p))));
				}
			}
		}

		void num_aretes() {
			int nA = -1;
			for (int i = 0; i < nbFaces; i++)
				for (int j = 0; j < 3; j++) {
					Arete tA = Arete(i, voisins[i][j].nF);
					if (find(aretes.begin(), aretes.end(), tA) == aretes.end()) {
						nA++;
						tA.nA = nA;
						aretes.push_back(tA);
					}
				}
		}

		void affiche_points () {
			cout << "NB POINTS : " << pts.size() << endl;
			int n = 0;
			for (auto&& el : pts) {
				cout << n++ << ": " << el << endl;
			}
		}

		void affiche_triangles () {
			cout << "NB TRIANGLES : " << t2d.size() << endl;
			int n = 0;
			for (auto&& el : t2d) {
				cout << n++ << ": " << el << endl;
			}
		}

		void affiche_faces () {
			int n = 0;
			cout << "NB FACES : " << faces.size() << endl;
			for (auto&& elem : faces) {
				cout << n << ": ";
				for (auto&& el : elem)
					cout << el << " ";
				cout << endl;
				n++;
			}
		}
		void affiche_groupes () {
			cout << "groupes :" << endl;
			int n = 0;
			for (auto&& elem : groupes) {
				cout << n << ": '" << elem << "'" << endl;
				n++;
			}
		}

		void affiche_aretes () {
			cout << "ARETES" << endl;
			int i = 0;
			for (auto&& el : aretes) {
				cout << i << ": " << el.nMin << " - " << el.nMax << " - " << el.nA << endl;
				i++;
			}
		}

		void affiche_voisinage () {
			cout << "VOISINAGE" << endl;
			for (int i = 0; i < (int)faces.size(); i++) {
				cout << i << ": ";
				for (int j = 0; j < 3; j++)
					cout << " " << voisins[i][j].nF;
				cout << endl;
			}
		}

		void charge() {
			fstream fsFichierOBJ(fichierOBJ);
			string strTampon;
			while (getline (fsFichierOBJ, strTampon))
				vLignes.push_back(strTampon);
			fsFichierOBJ.close();

			groupes.push_back("");
			int gCourant = 0;

			for (auto&& elem : vLignes) {
				if (elem.starts_with("v "))
					pts.push_back(lit_points(elem));
				else
				if (elem.starts_with("f "))
					faces.push_back(lit_faces(elem, gCourant));
				else
				if (elem.starts_with("g ")) {
					if (!groupes[0].empty()) {
						groupes.push_back("");
						gCourant++;
					}
				}
				else
				if (elem.starts_with("usemtl ")) {
					stringstream ss(elem);
					string s;
					ss>> s;
					ss>> s;
					groupes.back() = s;
				}
			}
			vLignes.clear();
			nbFaces = faces.size();
		}

	public :
		void affiche_depl () {
			cout << "DEPLIAGE" << endl;
			for (int i = 0; i < (int)facettes.size(); i++) {
				Facette f = facettes[i];
				cout << f.id << " " << f.groupe << endl;
				//cout << " " << f.page << " "<< f.piece << " " << f.orig << endl;
			}
			cout << endl;
		}

		static Vec3 lit_points (string ch) {
			Vec3 r;
			stringstream ss(ch);
			string el;

			int n = 0;
			while (ss >> el) {
						 if (n == 1) r.x = stof(el);
				else if (n == 2) r.y = stof(el);
				else if (n == 3) r.z = stof(el);
				n++;
			}
			return r;
		}

		static vector<int> lit_faces (string ch, int g) {
			vector<int> r;
			stringstream ss(ch);
			string el;
			
			int n = 0;
			while (ss >> el) {
				if (n > 0) {
					size_t f = el.find("/");
					string s = el.substr(0, f);
					 r.push_back(stoi(s)-1);
				}
				n++;
				
			}
			r.push_back(g);
			return r;
		}
		void init_depliage() {
			for (int i = 0; i < nbFaces; i++) {
				Facette d = Facette(i, t2d[i], faces[i][3]);
				facettes.push_back(d);
			}
		}

		int getNbFaces() { return nbFaces;}

		Donnees(string f) : fichierOBJ(f) {
			charge();
			init_triangles();
			calc_voisinage();
			calc_copl();
			num_aretes();
			init_depliage();

			// depliage
			for (int nG = 0; nG < (int)groupes.size(); nG++) {
				Facette* prochainLibre = premFaceLibre(nG);
				if (prochainLibre)
					//cout << facettes[prochainLibre].id << endl;
					cout << prochainLibre->id << endl;
				else
					cout << "vide" << endl;
				
			}
			
			
			
			/*SVG::SVG root;
			root.set_attr("width", "210");
			root.set_attr("height", "297");
			root.set_attr("viewBox", "0 0 210 297");
			
			auto shapes = root.add_child<SVG::Group>();
			
			// *shapes << SVG::Path(-100, -100, 100);
			
			auto rect = shapes->add_child<SVG::Path>("p1");
			auto t = depliage[0].triangle;
			rect->start(t.a.x, t.a.y);
			rect->line_to(t.b.x, t.b.y);
			rect->line_to(t.c.x, t.c.y);
			rect->to_origin();
			
			// Automatically scale width and height to fit elements
			//root.autoscale();

			// Output our drawing
			std::ofstream outfile("test.svg");
			outfile << std::string(root);
			*/
		}

		void affiche() {
			affiche_triangles();
			//affiche_points();
			//affiche_faces();
			//affiche_groupes();
			//affiche_voisinage();
			//affiche_aretes();
		}
};

int main (int argc, char** argv) {
	// Le volume est lu depuis un fichier au format .OBJ
	
	Donnees donnees(argv[1]); // Creation du dépliage
	
	//donnees.affiche();
	donnees.affiche_depl();
}
