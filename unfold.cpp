#include "unfold.h"

std::ostream& operator <<(std::ostream& os, const Vec2& v) {
    os << v.x << " " << v.y;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Vec3& v) {
    os << v.x << "," << v.y << "," << v.z;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Triangle2d& t) {
    os << t.a << " " << t.b << " " << t.c;
    return os;
}

std::ostream& operator <<(std::ostream& os, const Triangle3d& t) {
    os << t.a << "," << t.b << "," << t.c;
    return os;
}

Facette* Unfold::FirstFreeFacette(int g) {
    for(auto&& f : facettes) {
        if(f.group == g && !f.pose)
            return &f;
    }
    return nullptr;
}

void Unfold::init_triangles() {
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

void Unfold::calc_neighbourhood() {
    std::array<Neighbor, 3> tmpV;
    int vi;
    bool ok;
    for(int i = 0; i < nbFaces; i++) {
        for(int j = 0; j < 3; j++) {
            vi = 0;
            ok = false;
            do {
                if(vi != i) {
                    for(int k = 0;(k < 3) && !ok; k++)
                        if((faces[vi][k] == faces[i][next(j)])
                            &&(faces[vi][next(k)] == faces[i][j])) {
                            tmpV[j] = Neighbor(j, vi, next(k));
                            if(j == 2)
                                neighbors.push_back(tmpV);
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

void Unfold::calc_copl() {
    int p;
    for(int i = 0; i < nbFaces; i++) {
        for(int j = 0; j < 3; j++) {
            int nV = neighbors[i][j].nF;
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

void Unfold::num_edges() {
    int nE = -1;
    for(int i = 0; i < nbFaces; i++)
        for(int j = 0; j < 3; j++) {
            Edge tE = Edge(i, neighbors[i][j].nF);
            if(find(edges.begin(), edges.end(), tE) == edges.end()) {
                nE++;
                tE.nE = nE;
                edges.push_back(tE);
            }
        }
}

void Unfold::display_points() {
    std::cout << "NB POINTS : " << pts.size() << std::endl;
    int n = 0;
    for(auto&& el : pts)
        std::cout << n++ << ": " << el << std::endl;
}

void Unfold::display_triangles() {
    std::cout << "NB TRIANGLES : " << t2d.size() << std::endl;
    int n = 0;
    for(auto&& el : t2d)
        std::cout << n++ << ": " << el << std::endl;
}

void Unfold::display_faces() {
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

void Unfold::display_groups() {
    std::cout << "groupes :" << std::endl;
    int n = 0;
    for(auto&& elem : groups) {
        std::cout << n << ": '" << elem << "'" << std::endl;
        n++;
    }
}

void Unfold::display_edges() {
    std::cout << "ARETES" << std::endl;
    int i = 0;
    for(auto&& el : edges) {
        std::cout << i << ": " << el.nMin << " - " << el.nMax
                  << " - " << el.nE << std::endl;
        i++;
    }
}

void Unfold::display_neighbourhood() {
    std::cout << "neighbourhood" << std::endl;
    for(int i = 0; i <(int)faces.size(); i++) {
        std::cout << i << ": ";
        for(int j = 0; j < 3; j++)
            std::cout << " " << neighbors[i][j].nF;
        std::cout << std::endl;
    }
}

void Unfold::load_OBJ() {
    std::fstream fsfnOBJ(fnOBJ);

    groups.push_back("");
    int gCourant = 0;

    std::string elem;
    while(getline(fsfnOBJ, elem)) {
        if(elem.starts_with("v ")) {
            pts.push_back(read_points(elem));
        } else if(elem.starts_with("f ")) {
            faces.push_back(read_faces(elem, gCourant));
        } else if(elem.starts_with("g ")) {
            if(!groups[0].empty()) {
                groups.push_back("");
                gCourant++;
            }
        } else if(elem.starts_with("usemtl ")) {
            std::stringstream ss(elem);
            std::string s;
            ss >> s;
            ss >> s;
            groups.back() = s;
        }
    }
    fsfnOBJ.close();
    nbFaces = faces.size();
}

void Unfold::load_DAT() {
    std::fstream fsfnDAT(fnDAT);

    std::string elem;
    Page page;
    Piece piece;
    Facette facette;

    while(getline(fsfnDAT, elem)) {
        if(elem.starts_with("P ")) {
            page = read_page(elem);
            pages.push_back(page);
        } else if(elem.starts_with("p ")) {
            piece = read_piece(elem);
            page.pieces.push_back(piece);
            pages[page.id] = page;
        } else if(elem.starts_with("f ")) {
            facette = read_facette(elem);
            facette.page = page.id;
            facette.piece = piece.id;
            facette.triangle = t2d[facette.id];
            if(facette.orig == -1) {
                if (fabs(piece.a) > epsilon) {
                    facette.triangle.rotate(facette.triangle.centroid(), piece.a * PI/180);
                }
            } else {
                Neighbor *n = facettes[facette.orig].findNeighbor(facette.id);
                if (!n) {
                    std::cout << "VOISIN inconnu " << facette.orig << " . " << facette.id << std::endl;
                } else {
                    Triangle2d triOrig = facettes[facette.orig].triangle;
                    facette.triangle += triOrig.point(n->id)
                        - facette.triangle.point(n->idx);

                    double angle = calc_angle(
                        triOrig.point(n->id),
                        triOrig.point(next(n->id)),
                        facette.triangle.point(prev(n->idx)));
                    facette.triangle.rotate(triOrig.point(n->id), angle);
                }
            }
            facettes[facette.id].triangle = facette.triangle;
            piece.ajouteFace(facettes[facette.id], facette.orig, page.id, piece.id);
            page.pieces.back() = piece;
            pages[page.id] = page;
        }
    }
    reajuste_pieces();
}

Piece* Unfold::pieceGetById(std::vector<Piece> &pieces, int id) {
    for(auto&& p : pieces) {
        if(p.id == id)
            return &p;
    }
    return nullptr;
}

void Unfold::display_facettes(std::ostream &os) {
    os << "FICHIER OBJ : " << fnOBJ << std::endl;

    for(auto&& f : facettes) {
        os << f.id << "," << f.orig << "," << f.page << "," << f.piece
           << ", (" << f.neighbors[0].nF << "," << f.neighbors[1].nF << "," << f.neighbors[2].nF
           << ") ," << f.triangle;
        os << std::endl;
    }
}

void Unfold::display_unfold(std::ostream &os) {
    if (&os == &std::cout) {
        os << "unfolding" << std::endl;
        for(auto&& page : pages) {
            os << "PAGE " << page.id << std::endl;
            for(auto&& piece : page.pieces) {
                os << "..PIECE " << piece.id << " " << piece.O << " " << piece.a << std::endl;
                for(auto && fid : piece.faceId) {
                    Facette face = facettes[fid];
                    os << "....FACE " << face.id << " orig: " << face.orig
                       << " page: " << face.page << " piece: " << face.piece
                       << std::endl;
                }
            }
        }
    } else {
        os << fnOBJ << std::endl;
        for(auto&& page : pages) {
            os << "P " << page.id << std::endl;
            for(auto&& piece : page.pieces) {
                os << "p " << piece.id << " " << piece.O << std::endl;
                for(auto && fid : piece.faceId) {
                    Facette f = facettes[fid];
                    os << "f " << f.id << " " << f.orig
                    //<< " " << f.page << " " << f.piece
                    << std::endl;
                }
            }
        }
    }
}

Vec3 Unfold::read_points(std::string ch) {
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

std::vector<int> Unfold::read_faces(std::string ch, int g) {
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

Page Unfold::read_page(std::string ch) {
    Page page;
    std::stringstream ss(ch);

    char T;
    ss >> T >> page.id;

    return page;
}
Piece Unfold::read_piece(std::string ch){
    Piece piece;
    piece.id = 0;
    std::stringstream ss(ch);

    char T;
    //piece.O = Vec2(0.0, 0.0);

    ss >> T >> piece.id >> piece.O.x >> piece.O.y >> piece.a;
    return piece;
}
Facette Unfold::read_facette(std::string ch){
    Facette f;
    std::stringstream ss(ch);

    char T;
    ss >> T >> f.id >> f.orig;

    return f;
}

void Unfold::init_unfolding() {
    for(int i = 0; i < nbFaces; i++) {
        Facette d = Facette();
        d.id = i;
        d.orig = -1;
        d.page = -1;
        d.piece = -1;
        d.triangle = t2d[i];
        for(int j = 0; j < 3; j++)
            d.faces[j] = faces[i][j];
        d.group = faces[i][3];
        d.neighbors = neighbors[i];
        d.pose = false;
        facettes.push_back(d);
    }
}

int Unfold::pieceNextID() {
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

void Unfold::unfolding() {
    for(int nG = 0; nG <(int)groups.size(); nG++) {
        Facette* prochainLibre;
        do {
            prochainLibre = FirstFreeFacette(nG);
            if(prochainLibre) {
                Page page = Page(pages.size());
                Piece piece = Piece(pieceNextID());
                piece.ajouteFace(*prochainLibre, -1, page.id, piece.id);

                bool ok;
                do {
                    ok = false;
                    for(int f : piece.faceId) {
                        Facette fr = facettes[f];
                        for(auto&& fv : fr.neighbors) {
                            if(facettes[fv.nF].group == nG && !facettes[fv.nF].pose) {
                                // rapprocher
                                facettes[fv.nF].triangle += fr.triangle.point(fv.id)
                                                           - facettes[fv.nF].triangle.point(fv.idx);
                                // tourner
                                double angle = calc_angle(
                                    fr.triangle.point(fv.id),
                                    fr.triangle.point(next(fv.id)),
                                    facettes[fv.nF].triangle.point(prev(fv.idx))
                                    );
                                facettes[fv.nF].triangle.rotate(fr.triangle.point(fv.id), angle);
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
    reajuste_pieces();
}

void Unfold::reajuste_pieces() {
    // reajustement sur(0,0)
    for(auto&& p : pages)
        for(auto&& pi : p.pieces) {
            for(auto&& fid: pi.faceId)
                facettes[fid].triangle -= pi.pMin;
            pi.pMax = pi.pMax - pi.pMin;
            pi.pMin = pi.pMin - pi.pMin;
        }
}

int Unfold::getNbFaces() { return nbFaces;}

Unfold::Unfold(std::string fOBJ, std::string fDAT, std::string fSVG) :
    fnOBJ(fOBJ), fnDAT(fDAT), fnSVG(fSVG) {

    load_OBJ();
    init_triangles();
    calc_neighbourhood();
    calc_copl();
    num_edges();
    init_unfolding();
}

void Unfold::create_SVG(std::string fichier) {
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

void Unfold::display() {
    //display_triangles();
    //display_points();
    //display_faces();
    //display_groups();
    //display_neighbourhood();
    //display_edges();
    display_facettes(std::cout);
}
