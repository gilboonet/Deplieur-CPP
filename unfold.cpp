#include "unfold.h"
#include "lineitem.h"
#include "titleitem.h"

#include <QCursor>
#include <QFont>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QList>
#include <QObject>
#include <QSlider>

struct fct {
    int id;
    int orig;
    bool pose;
};

struct ppos {
    int id;
    QVector2D pos;
};

std::ostream& operator <<(std::ostream& os, const Facette& f) {
    os << f.id << "," << f.orig << "," << f.page << "," << f.piece
       << ", (" << f.neighbors[0].nF << "," << f.neighbors[1].nF << "," << f.neighbors[2].nF
       << ") ," << f.triangle;
    return os;
}

std::ostream& operator <<(std::ostream& os, const QVector2D& v) {
    os << v.x() << " " << v.y();
    return os;
}

std::ostream& operator <<(std::ostream& os, const QVector3D& v) {
    os << v.x() << "," << v.y() << "," << v.z();
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

            copl.push_back(Copl(i, nV, sgn(t3d[i].isCoplanar(t3d[nV].point(p)))));
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

void Unfold::display_copl() {
    std::cout << "COPL" << std::endl;
    for(auto&& c : copl) {
        std::cout << c.nF << "\t" << c.nV << "\t" << c.cop << std::endl;
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
                    facette.triangle.rotate(facette.triangle.centroid(),degToRad(piece.a));
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
    for (auto&& p : pieces) {
        if (p.id == id)
            return &p;
    }
    return nullptr;
}

Facette* Unfold::getFacette(const int id) {
    for (auto&& f : facettes) {
        if (f.id == id)
            return &f;
    }
    return nullptr;
}

Piece* Unfold::getPiece(const int id) {
    for (auto&& pg : pages) {
        for (auto& p : pg.pieces) {
            if (p.id == id)
                return &p;
        }
    }
    return nullptr;
}

Page* Unfold::getPage(const int id) {
    for (auto&& p : pages) {
        if (p.id == id)
            return &p;
    }
    return nullptr;
}

Piece* Unfold::getPieceCourante() {
    for (auto&& pg : pages) {
        for (auto&& p : pg.pieces) {
            if (p.id == IdPieceCourante) {
                return &p;
            }
        }
    }
    return nullptr;
}

void Unfold::setPieceCourante(const int id, TitleItem *ti) {
    IdPieceCourante = id;
    titleItem = ti;
};

void Unfold::display_facettes(std::ostream &os) {
    os << "FICHIER OBJ : " << fnOBJ << std::endl;

    for (auto&& f : facettes) {
        os << f;
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

QVector3D Unfold::read_points(std::string ch) {
    QVector3D r;
    std::stringstream ss(ch);
    std::string el;

    int n = 0;
    while(ss >> el) {
        if(n == 1) r.setX(stof(el));
        else if(n == 2) r.setY(stof(el));
        else if(n == 3) r.setZ(stof(el));
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
    float x,y;
    ss >> T >> piece.id >> x >> y >> piece.a;
    piece.O.setX(x);
    piece.O.setY(y);
    //ss >> T >> piece.id >> piece.O >> piece.a;

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
    IdPieceCourante = -1;
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
        for(auto&& n : d.neighbors) {
            n.cop = getCopl(i, n.nF)->cop;
        }
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
    int xMax = 0;
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

                piece.O.setX(xMax);
                xMax += piece.pMax.x();
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

Unfold::Unfold() {}

Unfold::Unfold(std::string fOBJ, std::string fDAT, std::string fSVG,
    QGraphicsView* vue, QSlider* slider) :
    fnOBJ(fOBJ), fnDAT(fDAT), fnSVG(fSVG), rVue(vue), rSlider(slider) {

    deja = false;
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
    root.style("line").set_attr("fill", "none").set_attr("stroke-width", "0.15mm");
    root.style("line.C").set_attr("stroke", "red");
    root.style("line.M").set_attr("stroke", "brown")
        .set_attr("stroke-dasharray", "4");
    root.style("line.V").set_attr("stroke", "green")
        .set_attr("stroke-dasharray", "4 1 1 1");

    root.style("text").set_attr("text-anchor", "middle")
        .set_attr("dominant-baseline", "middle");

    root.style("text.T").set_attr("fill", "blue").set_attr("font-size", "4px");
    root.style("text.E").set_attr("fill", "black").set_attr("font-size", "3px");
    root.style("text.p").set_attr("fill", "Indigo").set_attr("font-size", "8px");

    std::vector<SVG::Text*> txts;
    std::vector<SVG::Line*> lines;

    int deltaP = 0;
    for(auto&& pg : pages) {
        for(auto&& pi : pg.pieces) {
            std::vector<sedge> edges;
            auto shapes = root.add_child<SVG::Group>();
            QVector2D total = QVector2D(0.0, 0.0);
            for(auto&& fid : pi.faceId) {
                Triangle2d tt = facettes[fid].triangle + QVector2D(deltaP, 0) + pi.O;
                total += tt.centroid();

                // facette's #
                SVG::Text* txt = shapes->add_child<SVG::Text>(
                    SVG_Point(tt.centroid()), std::to_string(fid));
                txt->set_attr("class", "T");
                txts.push_back(txt);

                // facette's edges and neighbors #
                for (int i = 0; i < 3; i++) {
                    bool ok = true;
                    QVector2D p1 = tt.point(i),
                              p2 = tt.point(next(i));
                    for (auto&& e: edges) {
                        if (((e.p1 == p1) && (e.p2 == p2))
                         || ((e.p1 == p2) && (e.p2 == p1))) {
                            e.nb++;
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        sedge edge;
                        edge.p1 = p1;
                        edge.p2 = p2;
                        edge.fid = fid;
                        edge.nid = i;
                        edge.nF = facettes[fid].neighbors[i].nF;
                        edge.nb = 1;
                        edges.push_back(edge);
                    }
                }
            }

            for (auto&& e:edges) {
                SVG::Line* line = shapes->add_child<SVG::Line>(
                    SVG_Point(e.p1), SVG_Point(e.p2));
                if (e.nb == 1) {
                    line->set_attr("class", "C");

                    QVector2D m = middle(e.p1, e.p2);
                    double ra = angle(e.p1, e.p2) - std::numbers::pi;
                    SVG::Text* txt = shapes->add_child<SVG::Text>(
                        0, -1, std::to_string(e.nF));
                    std::string rs ="translate (" + std::to_string(m.x())
                        + " " + std::to_string(m.y()) + ")"
                        + " rotate (" + std::to_string(radToDeg(ra)) + ")";
                    txt->set_attr("transform", rs);
                    txt->set_attr("class", "E");
                    txts.push_back(txt);
                } else {
                    int c = facettes[e.fid].findNeighbor(e.nF)->cop;
                    line->set_attr("class", c < 0 ? "V" : "M");
                }
                lines.push_back(line);
            }
            total = total / pi.faceId.size();
            SVG::Text* txt = shapes->add_child<SVG::Text>(total.x(), total.y(), std::to_string(pi.id));
            txt->set_attr("class", "p");
            txts.push_back(txt);
        }
        deltaP += 220;
    }

    std::ofstream outfile(fichier);
    outfile << std::string(root);
}

void Unfold::create_SVG(QString f) {
    create_SVG(f.toStdString());
}

Edge* Unfold::edgeGet(int fid, int nid) {
    for (auto&& e : edges) {
        if (e == Edge(fid, nid))
            return &e;
    }
    return nullptr;
}

void Unfold::rotatePieceCourante(int value, int delta) {
    Piece * piece = getPieceCourante();
    if (piece) {
        if (delta != 0)
            value = piece->a + delta;

        piece->a = value;
        titleItem->setRotation(value);
    }
}

void Unfold::deplacePieceCourante(int deltaX, int deltaY) {
    Piece * piece = getPieceCourante();
    if (piece) {
        piece->O.setX(piece->O.x() + deltaX);
        piece->O.setY(piece->O.y() + deltaY);

        displayUI();
    }
}

void Unfold::displayUI() {
    //if (rVue->scene()) {
    if (deja) {
        for (auto&& it : rVue->scene()->items()) {
            if (it->flags() && QGraphicsItem::ItemIsMovable) {
                TitleItem* ti = static_cast<TitleItem*>(it);
                Piece* pi = getPiece(ti->data(0).toInt());
                if (pi) {
                    pi->O = QVector2D(
                        pi->O.x() - ti->data(1).toFloat() + ti->pos().x(),
                        pi->O.y() - ti->data(2).toFloat() + ti->pos().y()
                    );
                    if (pi->O.y() < 0)
                        pi->O.setY(0);
                }
            }
        }
    } else {
        deja = true;
    }

    QGraphicsScene* scene = new QGraphicsScene;
    int dxP = 0;
    QFont fEdges("Courier", 9);
    int max_nO = 1;
    for (auto&& page : pages) {
        scene->addRect(cmpo(5), cmpo(5), cmpo(210-5), cmpo(297-5))->setPos(cmpo(dxP), 0);

        for (auto&& piece : page.pieces) {
            std::vector<sedge> pedges;
            TitleItem *tit = new TitleItem(QString::number(piece.id+1), this);
            tit->setRotation(piece.a);
            tit->setData(0, piece.id);
            scene->addItem(tit);
            bool hasLabel = false;

            QVector2D cTIT;
            for (auto&& fn : piece.faceId) {
                //Triangle2d tt = cmpo(facettes[fn].triangle + piece.O + QVector2D(dxP+5, 5));
                Triangle2d tt = cmpo(facettes[fn].triangle) + piece.O ;//QVector2D(0*dxP+5, 5));
                if (!hasLabel){                  
                    QVector2D b = QVector2D(tit->boundingRect().width(), tit->boundingRect().height()) / 2;
                    cTIT = tt.centroid() - b;
                    tit->setPos(cTIT.x(), cTIT.y());
                    tit->setData(1, cTIT.x());
                    tit->setData(2, cTIT.y());
                    hasLabel = true;
                }

                for (int i = 0; i < 3 ; i++){
                    // facette's edges and neighbors #
                    int nF = facettes[fn].neighbors[i].nF;
                    bool ok = true;
                    QVector2D p1 = tt.point(i) - cTIT,
                              p2 = tt.point(next(i)) - cTIT;
                    for (auto&& e: pedges) {
                        if (fn == e.nF && nF == e.fid)
                            if ((eq(e.p1, p1) && eq(e.p2, p2)) || (eq(e.p1, p2) && eq(e.p2, p1))) {
                                e.nb++;
                                ok = false;
                                break;
                            }
                    }
                    if (ok) {
                        sedge edge;
                        edge.p1 = p1;
                        edge.p2 = p2;
                        edge.fid = fn;
                        edge.nid = i;
                        edge.nF = nF;
                        edge.nb = 1;
                        pedges.push_back(edge);
                    }
                }
            }

            for (auto&& e : pedges) {
                if (e.nb == 1) {
                    Edge* ed = edgeGet(e.fid, e.nF);
                    if (ed->nO == -1)
                        ed->nO = max_nO++;
                }
                LineItem *li = new LineItem();
                li->setLine(e.p1.x(), e.p1.y(), e.p2.x(), e.p2.y());

                int cop = facettes[e.fid].findNeighbor(e.nF)->cop;
                if (e.nb == 1) {
                    li->setTypeLigne(cop < 0 ? TL_COUPE_V : (cop > 0) ? TL_COUPE_M : TL_COUPE_C);
                    li->setOutlineColor(Qt::red);
                    li->setOutlineWidth(2);
                    li->setCustomPen();
                    li->setCursor(QCursor(Qt::SizeHorCursor));
                    li->setData(0, 1);
                    li->setData(1, e.fid);
                    li->setData(2, e.nF);

                    //QGraphicsSimpleTextItem *ti = scene->addSimpleText(QString::number(edgeGet(e.nF, e.fid)->nO), fEdges);
                    QGraphicsSimpleTextItem *ti = scene->addSimpleText(QString::number(edgeGet(e.nF, e.fid)->nE), fEdges);
                    QVector2D b = QVector2D(ti->boundingRect().width()/2, ti->boundingRect().height());
                    ti->setTransformOriginPoint(b.x(),  b.y());
                    double ra = angle(e.p1, e.p2) - std::numbers::pi;
                    ti->setRotation(radToDeg(ra));
                    QVector2D c = middle(e.p1, e.p2) - b;
                    ti->setPos(c.x(), c.y());
                    ti->setParentItem(tit);
                } else {
                    li->setTypeLigne(cop < 0 ? TL_LIE_V : (cop > 0) ? TL_LIE_M : TL_LIE_C);
                    li->setOutlineColor((cop < 0) ? Qt::green : (cop > 0) ? QColor(180,0,0) : QColor(240,240,240)); //Qt::lightGray);
                    li->setOutlineWidth(1);
                    li->setCustomPen();
                    li->setCursor(QCursor(Qt::SplitHCursor));
                    li->setData(0, -1);
                    li->setData(1, e.fid);
                    li->setData(2, e.nF);
                }
                li->setParentItem(tit);
            }
        }
        dxP += 210;
    }
    rVue->setScene(scene);
}

void Unfold::display() {
    //display_triangles();
    //display_points();
    //display_faces();
    //display_groups();
    //display_neighbourhood();
    //display_edges();
    //display_facettes(std::cout);
    //display_copl();
    display_unfold(std::cout);
}

Copl* Unfold::getCopl(int F, int V) {
    for(auto&& C : copl) {
        if (C.nF == F && C.nV == V)
            return &C;
    }
    return nullptr;
}

void Unfold::stickPiece(int a, int b) {
    qInfo("stickPiece %d %d", a, b);
    Facette* fA = getFacette(a);
    Facette* fB = getFacette(b);
    Piece* pieceA = getPiece(fA->piece);
    Piece* pieceB = getPiece(fB->piece);
    Page* pageB = getPage(fB->page);

    bool ok; // A et B doivent être dans des pièces différentes
    ok = fA->piece != fB->piece;

    std::vector<fct> pool;
    std::vector<fct> newP;
    size_t maxP = 0;
    if (ok) {
      std::cout << "FACETTES A DEPLACER" << std::endl;
      for (auto&& f : facettes) {
        if (f.page == fB->page && f.piece == fB->piece && f.orig > -1) {
            //std::cout << (f.pose ? "1" : "0") << " " << f.id <<" " << f.orig << std::endl;
            fct fc;
            fc.id = f.id;
            fc.orig = f.orig;
            fc.pose = false;
            pool.push_back(fc);
            maxP++;
        }
      }

      // 1) commencer par la face b liée à a
      fct fc = {b, a, true};
      newP.push_back(fc);

      // 2) boucler sur pool et insérer si orig est dans newP
      bool lok;
      do {
          do {
            for (auto&& p : pool) {
                if (p.pose)
                    continue;
                lok = false;
                for (auto&& n : newP) {
                    if (p.orig == n.id) {
                        p.pose = true;
                        newP.push_back(p);
                        lok = true;
                        break;
                    }
                }
                if (lok) {
                    break;
                }
            }
          } while (lok && newP.size() <= maxP);

          // 3) boucler sur pool et insérer si orig est dans newP
          do {
            for (auto&& p : pool) {
                if (p.pose)
                    continue;
                lok = false;
                for (auto&& n : newP) {
                    if (p.id == n.id) {
                        p.pose = true;
                        lok = true;
                        break;
                    }
                }
                if (lok) {
                    //std::cout << "OK " << p.orig << " " << p.id << std::endl;
                    fct f2 = {p.orig, p.id, true};
                    newP.push_back(f2);
                    break;
                }
            }
          } while (lok && newP.size() <= maxP);
      } while (newP.size() <= maxP);

      // FIN) Afficher
//      std::cout << "POOL" << std::endl;
//      for (auto&& ap : pool)
//        std::cout << (ap.pose ? "1" : "0") << " " << ap.id <<" " << ap.orig << std::endl;

      std::cout << "RESULTAT" << std::endl;
      Facette facOrig;
      for (auto&& an : newP) {
        facettes[an.id].orig = an.orig;
        facettes[an.id].page = fA->page;
        facettes[an.id].piece = fA->piece;
        std::cout << (an.pose ? "1" : "0") << " " << an.id <<" " << an.orig << std::endl;
        facOrig = facettes[an.orig];
        std::cout << facettes[an.id] << std::endl;
        Neighbor *n = facOrig.findNeighbor(an.id);
        facettes[an.id].triangle +=
            facOrig.triangle.point(n->id) - facettes[an.id].triangle.point(n->idx);
        double angle = calc_angle(
            facOrig.triangle.point(n->id),
            facOrig.triangle.point(next(n->id)),
            facettes[an.id].triangle.point(prev(n->idx)));
        facettes[an.id].triangle.rotate(facOrig.triangle.point(n->id), angle);
        pieceA->ajouteFace(facettes[an.id], an.orig, fA->page, fA->piece);
      }
      // supprimer ancienne piece
      for (size_t i = 0; i < pageB->pieces.size(); i++) {
        if (pageB->pieces.at(i).id == pieceB->id) {
            pageB->pieces.erase(pageB->pieces.begin() + i);
            break;
        }
      }

      // recalcule les mins et maxs
      /*for (auto&& pg : pages) {
        for (auto&& pi : pg.pieces) {
            for (auto&& i : pi.faceId) {
                Triangle2d t = facettes[i].triangle;
                for (int j= 0; i < 3; j++) {
                    if (pi.pMin.x() > t.point(j).x()) pi.pMin.setX(t.point(j).x());
                    if (pi.pMin.y() > t.point(j).y()) pi.pMin.setY(t.point(j).y());
                    if (pi.pMax.x() < t.point(j).x()) pi.pMax.setX(t.point(j).x());
                    if (pi.pMax.y() < t.point(j).y()) pi.pMax.setY(t.point(j).y());
                }
            }
        }
      }*/
      reajuste_pieces();
      displayUI();
    }
}

void Unfold::splitPiece(int a, int b) {
    qInfo("splitPiece %d %d", a, b);
    //qInfo() << a;
    //qInfo() << b;

    Facette* fA = getFacette(a);

    std::vector<fct> pool;
    std::vector<fct> newP;

    size_t maxP = 0;

    for (auto&& f : facettes) {
        if (f.page == fA->page) {
            fct fc;
            fc.id = f.id;
            fc.orig = f.orig;
            fc.pose = false;
            pool.push_back(fc);
            maxP++;
        }
    }

    //std::cout << "FACETTES A TESTER" << std::endl;
    //for (auto&& p : pool )
    //    std::cout << p.pose << " " << p.id << " " << p.orig << std::endl;

    bool ok = false;
    do {
        for (auto && p : pool) {
            if (p.pose)
                continue;
            if (p.id == b) {
                p.orig = -1;
                ok = true;
            } else {
                ok = false;
                for (auto&& n : newP)
                    if (p.orig == n.id) {
                        ok = true;
                        break;
                    }
            }
            if (ok) {
                p.pose = true;
                newP.push_back(p);
                break;
            }
            //ok = false;
        }
    } while (ok);

    //std::cout << "FACETTES DEPLACEES" << std::endl;
    //for (auto&& p : newP )
    //    std::cout << p.pose << " " << p.id << " " << p.orig << std::endl;

    int nPiece = pieceNextID();
    Piece* pieceA = getPiece(fA->piece);
    Piece nP = Piece(nPiece);
    nP.O = QVector2D(0,0);

    for (auto&& p : newP ) {
        nP.ajouteFace(facettes[p.id], p.orig, fA->page, nPiece);

        for (size_t i = 0 ; i < pieceA->faceId.size(); i++) {
            if (pieceA->faceId.at(i) == p.id) {
                pieceA->faceId.erase(pieceA->faceId.begin() + i);
                break;
            }
        }
    }
    getPage(fA->page)->ajoutePiece(nP);

    for (auto && f : pieceA->faceId)
        pieceA->recadre(facettes[f].triangle);

    displayUI();
}
