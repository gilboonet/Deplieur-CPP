#include "unfold.h"
#include "lineitem.h"
#include "titleitem.h"
#include "numitem.h"

#include <QCursor>
#include <QFont>
#include <QFontDatabase>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QList>
#include <QObject>
#include <QSize>
#include <QSvgGenerator>

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
            pts[faces[i][0]], pts[faces[i][1]], pts[faces[i][2]]);
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
    for(size_t i = 0; i < faces.size(); i++) {
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
    Flap flap;

    while(getline(fsfnDAT, elem)) {
        if(elem.starts_with("P ")) {
            page = read_page(elem);
            pages.push_back(page);
        } else if(elem.starts_with("p ")) {
            piece = read_piece(elem);
            page.pieces.push_back(piece);
            pages[page.id] = page;
        } else if(elem.starts_with("F ")) {
            facette = read_Premfacette(elem);
            facette.page = page.id;
            facette.piece = piece.id;
            getFacette(facette.id)->triangle = facette.triangle;
            piece.ajouteFace(*getFacette(facette.id), facette.orig, page.id, piece.id);
            page.pieces.back() = piece;
            pages[page.id] = page;
        } else if(elem.starts_with("f ")) {
            facette = read_facette(elem);
            facette.page = page.id;
            facette.piece = piece.id;
            facette.triangle = t2d[facette.id];
            Neighbor *n = getFacette(facette.orig)->findNeighbor(facette.id);
            if (!n) {
                std::cout << "VOISIN inconnu " << facette.orig << " . " << facette.id << std::endl;
            } else {
                Triangle2d triOrig = getFacette(facette.orig)->triangle;
                facette.triangle += triOrig.point(n->id)
                    - facette.triangle.point(n->idx);

                float angle = calc_angle(
                    triOrig.point(n->id),
                    triOrig.point(next(n->id)),
                    facette.triangle.point(prev(n->idx)));
                facette.triangle.rotate(triOrig.point(n->id), angle);
            }
            getFacette(facette.id)->triangle = facette.triangle;
            piece.ajouteFace(*getFacette(facette.id), facette.orig, page.id, piece.id);
            page.pieces.back() = piece;
            pages[page.id] = page;
        } else if (elem.starts_with("L ")) {
            read_langMode(elem);
        } else if (elem.starts_with("l ")) {
            flap = read_flap(elem);
            flaps.push_back(flap);
        }
    }
    //reajuste_pieces();
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
    for (auto&& page : pages) {
        for (auto& piece : page.pieces) {
            if (piece.id == id)
                return &piece;
        }
    }
    return nullptr;
}

Flap* Unfold::getFlap(const int f, const int n) {
    for (auto&& flap : flaps) {
        if (flap.fId == f && flap.nId == n)
            return &flap;
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

struct pd {
    int pid;
    int pg;
    int pc;
};

void Unfold::reducePages() {
    //qInfo() << "PAGINATION";
    std::vector <pd> nouvPage;
    for (auto&& pg : pages) {
        for (auto&& pi : pg.pieces) {
            QVector2D c = pi.O + (pi.pMax - pi.pMin);
            int p = c.x() / cmpo(pageDim.x());
            //qInfo() << "piece " << (pi.id+1) << " " << p << " " << pg.id;
            if (p != pg.id) {
                pd d = {pi.id, pg.id, p};
                nouvPage.push_back(d);
            }
        }
    }
    // change pages
    for (auto&& n : nouvPage) {
        Piece p = *getPiece(n.pid);
        std::erase_if(getPage(n.pg)->pieces, [&](Piece p) {return p.id == n.pid;});
        getPage(n.pc)->ajoutePiece(p);
    }
}

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
            if (page.pieces.size() > 0) {
                os << "PAGE " << page.id << std::endl;
                for(auto&& piece : page.pieces) {
                    os << "..PIECE " << piece.id << " " << piece.O << std::endl;
                    for(auto && fid : piece.faceId) {
                        Facette* face = getFacette(fid);
                        os << "....FACE " << face->id << " orig: " << face->orig
                           << " page: " << face->page << " piece: " << face->piece
                           << std::endl;
                    }
                }
            }
        }
    } else {
        os << fnOBJ << std::endl;
        int nbPi = 0;
        int nbPg = 0;
        for(auto&& page : pages) {
            if (page.pieces.size() > 0) {
                os << "P " << nbPg++ << std::endl;
                for(auto&& piece : page.pieces) {
                    os << "p " << nbPi++ << " " << piece.O << std::endl;
                    for(auto && fid : piece.faceId) {
                        Facette* f = getFacette(fid);
                        if (f->orig == -1) {
                            os << "F " << f->id
                            << " " << f->triangle.a.x() << " " << f->triangle.a.y()
                            << " " << f->triangle.b.x() << " " << f->triangle.b.y()
                            << " " << f->triangle.c.x() << " " << f->triangle.c.y()
                            << std::endl;
                        } else {
                            os << "f " << f->id << " " << f->orig << std::endl;
                        }
                    }
                }
            }
        }
        os << "L " << modeLanguettes << std::endl;
        if (modeLanguettes == 1) {
            for (auto&& l : flaps) {
                os << "l " << l.fId << " " << l.nId << " " << l.state << std::endl;
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

void Unfold::read_langMode(std::string ch) {
    std::stringstream ss(ch);

    char T;
    ss >> T >> modeLanguettes;
}

Flap Unfold::read_flap(std::string ch) {
    Flap flap;
    std::stringstream ss(ch);

    char T;
    ss >> T >> flap.fId >> flap.nId >> flap.state;

    return flap;
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
    ss >> T >> piece.id >> x >> y;
    piece.O.setX(x);
    piece.O.setY(y);

    return piece;
}

Facette Unfold::read_Premfacette(std::string ch){
    Facette f;
    std::stringstream ss(ch);

    char T;
    std::string sax, say, sbx, sby, scx, scy;
    ss >> T >> f.id >> sax >> say >> sbx >> sby >> scx >> scy;
    f.orig = -1;
    f.triangle.a.setX(stof(sax));
    f.triangle.a.setY(stof(say));
    f.triangle.b.setX(stof(sbx));
    f.triangle.b.setY(stof(sby));
    f.triangle.c.setX(stof(scx));
    f.triangle.c.setY(stof(scy));

    return f;
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

void Unfold::init_flaps() {
    flaps.clear();
    if (modeLanguettes != 1)
        return;

    for (auto&& f : facettes) {
        for (auto&& n : f.neighbors) {
            bool ok = true;
            for (auto&& fl : flaps) {
                if ((fl.fId == f.id && fl.nId == n.nF)
                 || (fl.fId == n.nF && fl.nId == f.id)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                Flap flap = Flap(f.id, n.nF, 1);
                flaps.push_back(flap);
            }
        }
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
    for(size_t nG = 0; nG < groups.size(); nG++) {
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
                        Facette* fr = getFacette(f);
                        for(auto&& fv : fr->neighbors) {
                            Facette* fn = getFacette(fv.nF);
                            if(fn->group == (int)nG && !fn->pose) {
                                // rapprocher
                                fn->triangle += fr->triangle.point(fv.id) - fn->triangle.point(fv.idx);
                                // tourner
                                float angle = calc_angle(
                                    fr->triangle.point(fv.id),
                                    fr->triangle.point(next(fv.id)),
                                    fn->triangle.point(prev(fv.idx))
                                    );
                                fn->triangle.rotate(fr->triangle.point(fv.id), angle);
                                // collision ?
                                ok = true;
                                for(auto&& t : piece.faceId)
                                    if(getFacette(t)->overlap(*fn)) {
                                        ok = false;
                                        break;
                                    }
                                // ajouter
                                if(ok) {
                                    piece.ajouteFace(*fn, f, page.id, piece.id);
                                    break;
                                }
                            }
                        }
                        if(ok)
                            break;
                    }
                }while(ok);

                //qInfo() << piece.pMax.x() - piece.pMin.x();
                piece.O.setX(xMax);
                xMax += cmpo(piece.pMax.x() - piece.pMin.x()+1);
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
                getFacette(fid)->triangle -= pi.pMin;
            pi.pMax = pi.pMax - pi.pMin;
            pi.pMin = pi.pMin - pi.pMin;
        }
}

int Unfold::getNbFaces() { return nbFaces;}

Unfold::Unfold() {}

Unfold::Unfold(std::string fOBJ, std::string fDAT, std::string fSVG,
    QGraphicsView* vue) :
    fnOBJ(fOBJ), fnDAT(fDAT), fnSVG(fSVG), rVue(vue) {

    deja = false;
    optimiserNums = false;
    modeLanguettes = 0;
    hLanguettes = 15;
    load_OBJ();
    init_triangles();
    calc_neighbourhood();
    calc_copl();
    num_edges();
    init_unfolding();
}

Edge* Unfold::edgeGet(int fid, int nid) {
    for (auto&& e : edges) {
        if (e == Edge(fid, nid))
            return &e;
    }
    return nullptr;
}

void Unfold::recalculeNums() {
    int n = 0;
    for (auto&& e : edges) {
        if (e.nO > -1) {
            e.nO = n++;
        }
    }
}

void Unfold::rotatePieceCourante(int delta) {
    Piece * piece = getPieceCourante();
    if (piece) {
        int f1 = piece->faceId.front();

        getFacette(f1)->triangle.rotate(getFacette(f1)->triangle.centroid(), degToRad(delta));
        for(size_t i = 1; i < piece->faceId.size(); i++) {
            Facette* facette = getFacette(piece->faceId[i]);
            Neighbor *n = getFacette(facette->orig)->findNeighbor(facette->id);
            Triangle2d triOrig = getFacette(facette->orig)->triangle;
            facette->triangle += triOrig.point(n->id) - facette->triangle.point(n->idx);

            float angle = calc_angle(
                triOrig.point(n->id),
                triOrig.point(next(n->id)),
                facette->triangle.point(prev(n->idx)));
            facette->triangle.rotate(triOrig.point(n->id), angle);
        }
    }
    displayUI();
}

void Unfold::deplacePieceCourante(int deltaX, int deltaY) {
    Piece * piece = getPieceCourante();
    if (piece) {
        piece->O.setX(piece->O.x() + deltaX);
        piece->O.setY(piece->O.y() + deltaY);

        displayUI();
    }
}


void Unfold::syncUI() {
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
                    ti->setData(1, pi->O.x());
                    ti->setData(2, pi->O.y());
                }
            }
        }
    } else {
        deja = true;
    }
}

void Unfold::displayUI(QString svg) {
    if (deja) {
        syncUI();
    } else {
        deja = true;
    }

    reducePages();

    bool doSVG = false;
    QPainter painter;
    QSvgGenerator sortieSVG;
    QFont fTitre, fNum;
    QPen pTitre, pNum;
    QPainterPath lPath, cPath, vPath, mPath, tPath;

    if (!svg.isEmpty()) {
        doSVG = true;
        sortieSVG.setFileName(svg);

        sortieSVG.setSize(QSize(pageDim.x(), pageDim.y()) *2.833);
        sortieSVG.setViewBox(QRect(0, 0, cmpo(pageDim.x()), cmpo(pageDim.y())));
        painter.begin(&sortieSVG);

        const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

        fTitre = QFont(fixedFont.family(), 15);
        pTitre = QPen(Qt::blue, 2);
        fNum = QFont(fixedFont.family(), 9);
        pNum = QPen(Qt::black, 1);
    }

    QGraphicsScene* scene = new QGraphicsScene;
    flash = new QGraphicsLineItem;
    flash->setVisible(false);
    flash->setPen(QPen(Qt::green, 2));
    scene->addItem(flash);
    int dxP = 0;
    int fs = 0;
    int max_nO = 1;

    float lang_s = static_cast<float>(hLanguettes);
    float lang_dt = 0.45;

    if (modeLanguettes == 2) {
        fs = 1;
    }
    for (auto&& page : pages) {
        scene->addRect(cmpo(2.5), cmpo(2.5), cmpo(pageDim.x()-5), cmpo(pageDim.y()-5))->setPos(cmpo(dxP), 0);
        //if (doSVG)
        //    painter.drawRect(cmpo(2.5 + dxP), cmpo(2.5), cmpo(pageDim.x()-5), cmpo(pageDim.y()-5));

        for (auto&& piece : page.pieces) {
            std::vector<sedge> pedges;
            TitleItem *tit = new TitleItem(QString::number(piece.id+1), this);
            tit->setData(0, piece.id);
            scene->addItem(tit);
            // calcule emplacement au centre de la pièce
            QVector2D cTIT{};
            for (auto&& fn : piece.faceId)
                cTIT += getFacette(fn)->triangle.centroid();
            cTIT /= piece.faceId.size();
            //bool hasLabel = false;
            QVector2D tt = cmpo(cTIT) + piece.O + QVector2D(cmpo(5), cmpo(5));
            QVector2D b = QVector2D(tit->boundingRect().width(), tit->boundingRect().height()) / 2;
            cTIT = tt - b;
            tit->setPos(cTIT.toPointF());
            tit->setData(1, cTIT.x());
            tit->setData(2, cTIT.y());
            if (doSVG) {
                QPainterPath pp;
                pp.addText(cTIT.toPointF(), fTitre, QString::number(piece.id+1));
                painter.setPen(pTitre);
                painter.drawPath(pp);
                //tPath.addPath(pp);
            }

            for (auto&& fn : piece.faceId) {
                Triangle2d tt = cmpo(getFacette(fn)->triangle) + piece.O + QVector2D(cmpo(5), cmpo(5));
                for (int i = 0; i < 3 ; i++){
                    // facette's edges and neighbors #
                    int nF = getFacette(fn)->neighbors[i].nF;
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
                li->setLine(QLineF(e.p1.toPointF(), e.p2.toPointF()));
                if (doSVG) {
                    lPath.clear();
                    lPath.moveTo(e.p1.toPointF());
                    lPath.lineTo(e.p2.toPointF());
                    lPath.translate(tit->pos());
                }

                int cop = getFacette(e.fid)->findNeighbor(e.nF)->cop;
                if (e.nb == 1) {
                    li->setData(0, 1);
                    li->setData(1, e.fid);
                    li->setData(2, e.nF);

                    if (modeLanguettes == 1) {
                        fs = 0;
                        Flap* flap = getFlap(e.fid, e.nF);
                        if (flap) {
                            fs = flap->state;
                        } else {
                            flap = getFlap(e.nF, e.fid);
                            if (flap) {
                                fs = -1 * flap->state;
                            }
                        }
                    }

                    if (fs == 1) {
                        li->setTypeLigne(cop < 0 ? TL_LIE_V : (cop > 0) ? TL_LIE_M : TL_LIE_C);
                        li->setOutlineColor((cop < 0) ? Qt::green : (cop > 0) ? QColor(180,0,0) : QColor(240,240,240)); //Qt::lightGray);
                        li->setOutlineWidth(1);
                        li->setCustomPen();
                        if (doSVG) {
                            if (li->pen().color() == Qt::green) {
                                vPath.addPath(lPath);
                            } else if (li->pen().color() == QColor(180,0,0)) {
                                mPath.addPath(lPath);
                            }
                        }

                        float d = distance(e.p1, e.p2);
                        float dt = lang_dt;
                        if (d > 50) dt = dt/2;
                        float a = degToRad(90) - direction(e.p1, e.p2);

                        QVector2D P1 = rotatePt(e.p1 + QVector2D(lang_s, d*dt), e.p1, a);
                        QVector2D P2 = rotatePt(e.p1 + QVector2D(lang_s, d*(1-dt)), e.p1, a);

                        QPainterPath path;
                        path.moveTo(e.p1.toPointF());
                        path.lineTo(P1.toPointF());
                        path.lineTo(P2.toPointF());
                        path.lineTo(e.p2.toPointF());
                        QGraphicsPathItem* pi = scene->addPath(path);
                        pi->setZValue(0);
                        pi->setParentItem(tit);
                        pi->setPen(QPen(Qt::red, 2));
                        if (doSVG) {
                            path.translate(tit->pos());
                            cPath.addPath(path);
                        }
                    } else {
                        li->setTypeLigne(cop < 0 ? TL_COUPE_V : (cop > 0) ? TL_COUPE_M : TL_COUPE_C);
                        li->setOutlineWidth(2);
                        li->setCursor(QCursor(Qt::SizeHorCursor));
                        li->setOutlineColor(Qt::red);
                        li->setCustomPen();
                        if (doSVG) {
                            cPath.addPath(lPath);
                        }
                    }

                    Edge * edg = edgeGet(e.nF, e.fid);
                    if (edg) {
                        int n = optimiserNums ? edg->nO : edg->nE;
                        NumItem *ti = new NumItem(QString::number(n), e.nF, e.fid);
                        QVector2D b = QVector2D(ti->boundingRect().width()/2, ti->boundingRect().height());
                        ti->setTransformOriginPoint(b.toPointF());
                        float ra = angle(e.p1, e.p2) - std::numbers::pi;
                        ti->setRotation(radToDeg(ra));
                        QVector2D c = middle(e.p1, e.p2) - b;
                        ti->setPos(c.toPointF());
                        ti->setZValue(0);
                        ti->setParentItem(tit);
                        if (doSVG) {
                            QPainterPath pp;
                            pp.addText(- ti->boundingRect().width()/2, -2, fNum, QString::number(n));
                            QTransform tr;
                            QPointF pt = c.toPointF()+ b.toPointF() +tit->pos();
                            tr.translate(pt.x(), pt.y());
                            tr.rotate(radToDeg(ra));
                            tPath.addPath(tr.map(pp));
                        }
                    }
                } else {
                    li->setLine(QLineF(e.p1.toPointF(), e.p2.toPointF()));
                    li->setTypeLigne(cop < 0 ? TL_LIE_V : (cop > 0) ? TL_LIE_M : TL_LIE_C);
                    li->setOutlineColor((cop < 0) ? Qt::green : (cop > 0) ? QColor(180,0,0) : QColor(240,240,240)); //Qt::lightGray);
                    li->setOutlineWidth(1);
                    li->setCustomPen();
                    li->setCursor(QCursor(Qt::SplitHCursor));
                    li->setData(0, -1);
                    li->setData(1, e.fid);
                    li->setData(2, e.nF);
                    if (doSVG) {
                        if (li->pen().color() == Qt::green) {
                            vPath.addPath(lPath);
                        } else if (li->pen().color() == QColor(180,0,0)) {
                            mPath.addPath(lPath);
                        }
                    }
                }
                li->setZValue(0);
                li->setParentItem(tit);
            }
            tit->setZValue(1);
        }
        if (doSVG) {
            if (!tPath.isEmpty()) {
                QPen p = QPen(Qt::black, 1);
                p.setStyle(Qt::SolidLine);
                painter.setPen(p);
                painter.drawPath(tPath);
                tPath.clear();
            }
            if (!cPath.isEmpty()) {
                QPen p = QPen(Qt::red, 1);
                p.setStyle(Qt::SolidLine);
                painter.setPen(p);
                painter.drawPath(cPath);
                cPath.clear();
            }
            if (!vPath.isEmpty()) {
                QPen p = QPen(Qt::green, 1);
                QVector<qreal> dashes;
                dashes << 8 << 2 << 1 << 2;
                p.setStyle(Qt::CustomDashLine);
                p.setDashPattern(dashes);
                painter.setPen(p);
                painter.drawPath(vPath);
                vPath.clear();
            }
            if (!mPath.isEmpty()) {
                QPen p = QPen(QColor(180,0,0), 1);
                QVector<qreal> dashes;
                dashes << 8 << 8;
                p.setStyle(Qt::CustomDashLine);
                p.setDashPattern(dashes);
                painter.setPen(p);
                painter.drawPath(mPath);
                mPath.clear();
            }
        }
        dxP += pageDim.x();
    }
    if (doSVG) {
        painter.end();
    }

    rVue->setScene(scene);
    reducePages();
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
    //qInfo("stickPiece %d %d", a, b);
    Facette* fA = getFacette(a);
    Facette* fB = getFacette(b);
    Piece* pieceA = getPiece(fA->piece);
    Piece* pieceB = getPiece(fB->piece);
    Page* pageB = getPage(fB->page);

    if (fA->piece == fB->piece) {
        qInfo() << "STICK erreur : aretes d'une meme piece";
        return;
    }

    std::vector<fct> pool;
    std::vector<fct> newP;
    size_t maxP = 0;

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
  for (auto&& an : newP) {
    Facette *fn = getFacette(an.id);
    fn->orig = an.orig;
    fn->page = fA->page;
    fn->piece = fA->piece;
    std::cout << (an.pose ? "1" : "0") << " " << an.id <<" " << an.orig << std::endl;
    Facette* facOrig = getFacette(an.orig);
    std::cout << *fn << std::endl;
    Neighbor *n = facOrig->findNeighbor(an.id);
    fn->triangle += facOrig->triangle.point(n->id) - fn->triangle.point(n->idx);
    float angle = calc_angle(
        facOrig->triangle.point(n->id),
        facOrig->triangle.point(next(n->id)),
        fn->triangle.point(prev(n->idx)));
    fn->triangle.rotate(facOrig->triangle.point(n->id), angle);
    pieceA->ajouteFace(*fn, an.orig, fA->page, fA->piece);
  }
  // supprimer ancienne piece
  std::erase_if(pageB->pieces, [&](Piece p) {return p.id == pieceB->id;});

  recalculeNums();
  displayUI();
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
    if (!pieceA) {
        qInfo() << "SPLIT : erreur piece " << fA->piece;
        return;
    }
    Piece nP = Piece(nPiece);
    nP.O = QVector2D(0,0);

    for (auto&& p : newP ) {
        Facette *pf = getFacette(p.id);
        if (!pf) {
            qInfo() << "SPLIT : erreur facette " << p.id;
            return;
        }
        nP.ajouteFace(*pf, p.orig, fA->page, nPiece);

        std::erase_if(pieceA->faceId, [&](int i) {return i == p.id;});
    }
    getPage(fA->page)->ajoutePiece(nP);

    recalculeNums();
    displayUI();
}

void Unfold::changeLanguette(int f, int n) {
    Flap* flap = getFlap(f, n);
    if (!flap)
        flap = getFlap(n, f);

    if (flap) {
        flap->state = -1 * flap->state;
        displayUI();
    }
}
