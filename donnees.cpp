#include "donnees.h"

std::ostream& operator <<(std::ostream& os, const Vec2& v) {
    //os << "(" << v.x << ", " << v.y << ")";
    os << v.x << "," << v.y;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Vec3& v) {
    //os << v.x << ", " << v.y << ", " << v.z;
    os << v.x << "," << v.y << "," << v.z;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Triangle2d& t) {
    os << t.a << "," << t.b << "," << t.c;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Triangle3d& t) {
    os << t.a << "," << t.b << "," << t.c;
    return os;
}

Facette* Donnees::premFaceLibre(int g) {
    for(auto&& f : facettes) {
        if(f.groupe == g && !f.pose)
            return &f;
    }
    return nullptr;
}

void Donnees::init_triangles() {
    for(int i = 0; i < nbFaces; i++) {
        Triangle3d t3di = Triangle3d(
            pts[faces[i][0]],
            pts[faces[i][1]],
            pts[faces[i][2]]);
        t3d.push_back(t3di);
        Triangle2d t2di = t3di.d2ize();
        t2d.push_back(t2di);
    }
}

void Donnees::calc_voisinage() {
    std::array<Voisin, 3> tmpV;
    int vi;
    bool ok;
    for(int i = 0; i < nbFaces; i++) {
        for(int j = 0; j < 3; j++) {
            vi = 0;
            ok = false;
            do {
                if(vi != i) {
                    for(int k = 0;(k < 3) && !ok; k++)
                        if((faces[vi][k] == faces[i][suiv(j)])
                            &&(faces[vi][suiv(k)] == faces[i][j])) {
                            tmpV[j] = Voisin(j, vi, suiv(k));
                            if(j == 2)
                                voisins.push_back(tmpV);
                            ok = true;
                        }
                    if(!ok)
                        vi++;
                } else
                    vi++;
            } while((vi < nbFaces) && !ok);
        }
    }
}

void Donnees::calc_copl() {
    int p;
    for(int i = 0; i < nbFaces; i++) {
        for(int j = 0; j < 3; j++) {
            int nV = voisins[i][j].nF;
            if(t3d[i].eq3(t3d[nV], 0))
                p = 0;
            else if(t3d[i].eq3(t3d[nV], 1))
                p = 1;
            else
                p = 2;

            copl.push_back(Copl(i, nV, t3d[i].isCoplanar(t3d[nV].point(p))));
        }
    }
}

void Donnees::num_aretes() {
    int nA = -1;
    for(int i = 0; i < nbFaces; i++)
        for(int j = 0; j < 3; j++) {
            Arete tA = Arete(i, voisins[i][j].nF);
            if(find(aretes.begin(), aretes.end(), tA) == aretes.end()) {
                nA++;
                tA.nA = nA;
                aretes.push_back(tA);
            }
        }
}

void Donnees::affiche_points() {
    std::cout << "NB POINTS : " << pts.size() << std::endl;
    int n = 0;
    for(auto&& el : pts)
        std::cout << n++ << ": " << el << std::endl;
}

void Donnees::affiche_triangles() {
    std::cout << "NB TRIANGLES : " << t2d.size() << std::endl;
    int n = 0;
    for(auto&& el : t2d)
        std::cout << n++ << ": " << el << std::endl;
}

void Donnees::affiche_faces() {
    int n = 0;
    std::cout << "NB FACES : " << faces.size() << std::endl;
    for(auto&& elem : faces) {
        std::cout << n << ": ";
        for(auto&& el : elem)
            std::cout << el << " ";
        std::cout << std::endl;
        n++;
    }
}

void Donnees::affiche_groupes() {
    std::cout << "groupes :" << std::endl;
    int n = 0;
    for(auto&& elem : groupes) {
        std::cout << n << ": '" << elem << "'" << std::endl;
        n++;
    }
}

void Donnees::affiche_aretes() {
    std::cout << "ARETES" << std::endl;
    int i = 0;
    for(auto&& el : aretes) {
        std::cout << i << ": " << el.nMin << " - " << el.nMax
                  << " - " << el.nA << std::endl;
        i++;
    }
}

void Donnees::affiche_voisinage() {
    std::cout << "VOISINAGE" << std::endl;
    for(int i = 0; i <(int)faces.size(); i++) {
        std::cout << i << ": ";
        for(int j = 0; j < 3; j++)
            std::cout << " " << voisins[i][j].nF;
        std::cout << std::endl;
    }
}

void Donnees::charge() {
    std::fstream fsFichierOBJ(fichierOBJ);
    std::string strTampon;
    while(getline(fsFichierOBJ, strTampon))
        vLignes.push_back(strTampon);
    fsFichierOBJ.close();

    groupes.push_back("");
    int gCourant = 0;

    for(auto&& elem : vLignes) {
        if(elem.starts_with("v "))
            pts.push_back(lit_points(elem));
        else
            if(elem.starts_with("f "))
                faces.push_back(lit_faces(elem, gCourant));
            else
                if(elem.starts_with("g ")) {
                    if(!groupes[0].empty()) {
                        groupes.push_back("");
                        gCourant++;
                    }
                }
                else
                    if(elem.starts_with("usemtl ")) {
                        std::stringstream ss(elem);
                        std::string s;
                        ss >> s;
                        ss >> s;
                        groupes.back() = s;
                    }
    }
    vLignes.clear();
    nbFaces = faces.size();
}

Piece* Donnees::pieceGetById(std::vector<Piece> &pieces, int id) {
    for(auto&& p : pieces) {
        if(p.id == id)
            return &p;
    }
    return nullptr;
}

void Donnees::affiche_facettes(std::ostream &os) {
    os << fichierOBJ << std::endl;
    os << facettes.size()<< std::endl;

    for(auto&& f : facettes) {
        os << f.id << "," << f.orig << "," << f.page << ","
            << f.piece << "," << f.triangle;
        os << std::endl;
    }
}

void Donnees::affiche_depl(std::ostream &os) {
    if (&os == &std::cout) {
        std::cout << "DEPLIAGE" << std::endl;
        for(auto&& page : pages) {
            std::cout << "PAGE " << page.id << std::endl;
            for(auto&& piece : page.pieces) {
                std::cout << "..PIECE " << piece.id << " " << piece.O << std::endl;
                for(auto && fid : piece.faceId) {
                    Facette face = facettes[fid];
                    std::cout << "....FACE " << face.id << " orig: " << face.orig
                              << " page: " << face.page << " piece: " << face.piece << std::endl;
                }
            }
        }
    } else {
        for(auto&& page : pages) {
            os << "P " << page.id << std::endl;
            for(auto&& piece : page.pieces) {
                os << "p " << piece.id << "," << piece.O << std::endl;
                for(auto && fid : piece.faceId) {
                    Facette f = facettes[fid];
                    os << "f " << f.id << "," << f.orig << "," << f.page
                       << "," << f.piece << "," << f.triangle << std::endl;
                }
            }
        }
    }
}

Vec3 Donnees::lit_points(std::string ch) {
    Vec3 r;
    std::stringstream ss(ch);
    std::string el;

    int n = 0;
    while(ss >> el) {
        if(n == 1) r.x = stof(el);
        else if(n == 2) r.y = stof(el);
        else if(n == 3) r.z = stof(el);
        n++;
    }
    return r;
}

std::vector<int> Donnees::lit_faces(std::string ch, int g) {
    std::vector<int> r;
    std::stringstream ss(ch);
    std::string el;

    int n = 0;
    while(ss >> el) {
        if(n > 0) {
            size_t f = el.find("/");
            std::string s = el.substr(0, f);
            r.push_back(stoi(s) - 1);
        }
        n++;

    }
    r.push_back(g);
    return r;
}

void Donnees::init_depliage() {
    for(int i = 0; i < nbFaces; i++) {
        Facette d = Facette();
        d.id = i;
        d.orig = -1;
        d.page = -1;
        d.piece = -1;
        d.triangle = t2d[i];
        for(int j = 0; j < 3; j++)
            d.faces[j] = faces[i][j];
        d.groupe = faces[i][3];
        d.voisins = voisins[i];
        d.pose = false;
        facettes.push_back(d);
    }
}

int Donnees::pieceProchainId() {
    int id = 0;
    bool trouve;
    do {
        trouve = false;
        for(auto&& page : pages) {
            if(pieceGetById(page.pieces, id)) {
                id++;
                trouve = true;
                break;
            }
        }
    } while(trouve);
    return id;
}

void Donnees::depliage() {
    for(int nG = 0; nG <(int)groupes.size(); nG++) {
        Facette* prochainLibre;
        //Page page;
        //Piece piece;
        do {
            prochainLibre = premFaceLibre(nG);
            if(prochainLibre) {
                Page page = Page(pages.size());
                Piece piece = Piece(pieceProchainId());
                piece.ajouteFace(*prochainLibre, -1, page.id, piece.id);

                bool ok;
                do {
                    ok = false;
                    for(int f : piece.faceId) {
                        Facette fr = facettes[f];
                        for(auto&& fv : fr.voisins) {
                            if(facettes[fv.nF].groupe == nG && !facettes[fv.nF].pose) {
                                // rapprocher
                                facettes[fv.nF].triangle += fr.triangle.point(fv.id)
                                                           - facettes[fv.nF].triangle.point(fv.idx);
                                // tourner
                                double angle = calc_angle(
                                    fr.triangle.point(fv.id),
                                    fr.triangle.point(suiv(fv.id)),
                                    facettes[fv.nF].triangle.point(prec(fv.idx))
                                    );
                                facettes[fv.nF].triangle.rotation(fr.triangle.point(fv.id), angle);
                                // collision ?
                                ok = true;
                                for(auto&& t : piece.faceId)
                                    if(facettes[t].overlap(facettes[fv.nF])) {
                                        ok = false;
                                        break;
                                    }
                                // ajouter
                                if(ok) {
                                    piece.ajouteFace(facettes[fv.nF], f, page.id, piece.id);
                                    break;
                                }
                            }
                        }
                        if(ok)
                            break;
                    }
                }while(ok);

                page.ajoutePiece(piece);
                pages.push_back(page);
            }
        } while(prochainLibre);
    }
    // reajustement sur(0,0)
    for(auto&& p : pages)
        for(auto&& pi : p.pieces) {
            for(auto&& fid: pi.faceId) {
                facettes[fid].triangle -= pi.pMin;
            }
            pi.pMax = pi.pMax - pi.pMin;
            pi.pMin = pi.pMin - pi.pMin;
        }

}

int Donnees::getNbFaces() { return nbFaces;}

Donnees::Donnees(std::string fOBJ, std::string fDAT, std::string fSVG) :
    fichierOBJ(fOBJ), fichierDAT(fDAT), fichierSVG(fSVG) {

    charge();
    init_triangles();
    calc_voisinage();
    calc_copl();
    num_aretes();
    init_depliage();
}

void Donnees::cree_SVG(std::string fichier) {
    SVG::SVG root;
    root.set_attr("width", "210");
    root.set_attr("height", "297");
    root.set_attr("viewBox", "0 0 210 297");

    // Basic CSS support
    root.style("path").set_attr("fill", "none")
        .set_attr("stroke", "black");
    //root.style("rect#my_rectangle").set_attr("fill", "red");
    root.style("text").set_attr("fill", "blue")
        .set_attr("font-size", "10px")
        .set_attr("text-anchor", "middle")
        .set_attr("dominant-baseline", "middle");


    auto shapes = root.add_child<SVG::Group>();
    std::vector<SVG::Path*> tris;
    std::vector<SVG::Text*> txts;


    int deltaP = 0;
    for(auto&& pg : pages) {
        for(auto&& pi : pg.pieces) {
            for(auto&& fid : pi.faceId) {
                Facette t = facettes[fid];
                SVG::Path* rect = shapes->add_child<SVG::Path>();
                //Vec2 dP = Vec2(deltaP, 0);
                auto tt = t.triangle + Vec2(deltaP, 0) + pi.O;
                rect->start(tt.a.x, tt.a.y);
                rect->line_to(tt.b.x, tt.b.y);
                rect->line_to(tt.c.x, tt.c.y);
                rect->to_origin();
                tris.push_back(rect);

                Vec2 c = tt.centroid();
                SVG::Text* txt = shapes->add_child<SVG::Text>(c.x, c.y, std::to_string(fid));
                txts.push_back(txt);
            }
        }
        deltaP += 220;
    }

    std::ofstream outfile(fichier);
    outfile << std::string(root);
}

void Donnees::affiche() {
    //affiche_triangles();
    //affiche_points();
    affiche_faces();
    //affiche_groupes();
    affiche_voisinage();
    //affiche_aretes();
}
