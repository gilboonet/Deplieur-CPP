#include "unfold.h"
#include "lineitem.h"
//#include "titleitem.h"
#include "numitem.h"
#include "pieceitem.h"

#include <QCursor>
#include <QFont>
#include <QFontDatabase>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QLineF>
#include <QList>
#include <QObject>
#include <QSize>

#include "../svg.hpp"

struct fct {
    int id;
    int orig;
    bool pose;
};

struct ppos {
    int id;
    QPointF pos;
};

std::ostream& operator <<(std::ostream& os, const Facette& f) {
    os << f.id << "," << f.orig << "," << f.page << "," << f.piece
       << ", (" << f.neighbors[0].nF << "," << f.neighbors[1].nF << "," << f.neighbors[2].nF
       << ") ," << f.triangle;
    return os;
}

std::ostream& operator <<(std::ostream& os, const QPointF& v) {
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

void Unfold::init_triangles(qreal ech) {
    for(auto&& p : pts) {
        p *= ech;
    }

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

void Unfold::load_OBJ(const QByteArray *donnees) {
    QTextStream tsOBJ(*donnees);

    groups.push_back("");
    int gCourant = 0;
    nbFaces = 0;

    QString elem = tsOBJ.readLine();
    while (!elem.isNull()) {
        std::string selem = elem.toStdString();
        if(selem.starts_with("v ")) {
            pts.push_back(read_points(selem));
        } else if(selem.starts_with("f ")) {
            std::vector<int> rf = read_faces(selem);
            std::vector<int> rfj;
            for(size_t j = 1; j < rf.size()-1; j++) {
                rfj.clear();
                rfj.push_back(rf[0]);
                rfj.push_back(rf[j]);
                rfj.push_back(rf[j+1]);
                rfj.push_back(gCourant);
                faces.push_back(rfj);
                nbFaces++;
            }
        }
        else if(selem.starts_with("usemtl ")) {
            std::stringstream ss(selem);
            std::string s;
            ss >> s;
            ss >> s;
            if(!groups[0].empty()) {
                groups.push_back("");
                gCourant++;
            }
            groups.back() = s;
        }
        elem = tsOBJ.readLine();
    }
}

void Unfold::load_DAT(const QByteArray *donnees) {
    QTextStream tsDAT(*donnees);

    Page page;
    Piece piece;
    Facette facette;
    Flap flap;
    QVector3D pt;
    std::vector<int> face;
    std::string groupe;

    // 1) Lecture donnees OBJ
    nbFaces = 0;
    QString elem = tsDAT.readLine();
    while (!elem.isNull()) {
        std::string selem = elem.toStdString();
        if(selem.starts_with("D ")) {
            pageId = read_pageId(selem);
            switch(pageId) {
                case 3 : pageFormat(3); break;
                case 5 : pageFormat(5); break;
                case 11 : pageFormat(11); break;
                case 12 : pageFormat(12); break;
                case 13 : pageFormat(13); break;
                case 14 : pageFormat(14); break;
                default : pageFormat(4);
            }
        }
        else if(selem.starts_with("v ")) {
            pt = read_points(selem);
            pts.push_back(pt);
        }
        else if(selem.starts_with("f ")) {
            face = read_facesDAT(selem);
            faces.push_back(face);
            nbFaces++;
        }
        else if(selem.starts_with("G ")) {
            groupe = read_group(selem);
            groups.push_back(groupe);
        }
        elem = tsDAT.readLine();
    }
    init_triangles(1.0);
    calc_neighbourhood();
    calc_copl();
    num_edges();
    init_unfolding();

    tsDAT.seek(0);
    elem = tsDAT.readLine();
    while (!elem.isNull()) {
        std::string selem = elem.toStdString();
        if(selem.starts_with("P ")) {
            page = read_page(selem);
            pages.push_back(page);
        }
        else if(selem.starts_with("p ")) {
            piece = read_piece(selem);
            page.pieces.push_back(piece);
            pages[page.id] = page;
        }
        else if(selem.starts_with("T ")) {
            facette = read_Premfacette(selem);
            facette.page = page.id;
            facette.piece = piece.id;
            getFacette(facette.id)->triangle = facette.triangle;
            piece.ajouteFace(*getFacette(facette.id), facette.orig, page.id, piece.id);
            page.pieces.back() = piece;
            pages[page.id] = page;
        }
        else if(selem.starts_with("t ")) {
            facette = read_facette(selem);
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

                qreal angle = calc_angle(
                    triOrig.point(n->id),
                    triOrig.point(next(n->id)),
                    facette.triangle.point(prev(n->idx)));
                facette.triangle.rotate(triOrig.point(n->id), angle);
            }
            getFacette(facette.id)->triangle = facette.triangle;
            piece.ajouteFace(*getFacette(facette.id), facette.orig, page.id, piece.id);
            page.pieces.back() = piece;
            pages[page.id] = page;
        }
        else if(selem.starts_with("L ")) {
            read_langMode(selem);
        }
        else if(selem.starts_with("l ")) {
            flap = read_flap(selem);
            flaps.push_back(flap);
        }
        elem = tsDAT.readLine();
    }
    qInfo() << "faces " << faces.size();
    qInfo() << "triangles depl " << facettes.size();
}

void Unfold::pageFormat(int n) {
    qreal x, y;

    switch(n) {
        case  3 : x = 297;      y = 420; break;
        case  5 : x = 148.5;    y = 210; break;
        case 11 : x = 305;      y = 610; break;
        case 12 : x = 305;      y = 305; break;
        case 13 : x = 115;      y = 305; break;
        case 14 : x = 115;      y = 165; break;
        default : x = 210;      y = 297;
    }

    pageDim = QPointF(x, y);
    pageId = n;
    displayUI();
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

void Unfold::setPieceCourante(const int id, PieceItem *ti) {
    IdPieceCourante = id;
    pieceItem = ti;
};

struct pd {
    int pid;
    int pg;
    int pc;
};

void Unfold::reducePages() {
    std::vector <pd> nouvPage;
    for (auto&& pg : pages) {
        for (auto&& pi : pg.pieces) {
            QPointF c = pi.O + (pi.pMax - pi.pMin);
            int p = c.x() / cmpo(pageDim.x());
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
        if (!getPage(n.pc)) {
            Page page;
            page.id = n.pc;
            page.ajoutePiece(p);
            pages.push_back(page);
        }
        else {
            getPage(n.pc)->ajoutePiece(p);
        }
        for (auto&& fid : p.faceId) {
            getFacette(fid)->page = n.pc;
            getFacette(fid)->piece = n.pid;
        }
    }
}

void Unfold::display_facettes(std::ostream &os) {
    os << "FICHIER OBJ : " << fnOBJ << std::endl;

    for (auto&& f : facettes) {
        os << f;
    }
}

void Unfold::save_unfold(std::stringstream &ts) {
    int nbPi = 0;
    int nbPg = 0;

    ts << "D " << pageId << std::endl;
    for(auto&& p : pts) {
        ts << "v " << p.x() << " " << p.y() << " " << p.z() << std::endl;
    }

    for(auto&& g : groups) {
        ts << "G " << g << std::endl;
    }

    for (auto&& f : faces) {
        ts << "f";
        for (int i = 0; i <= 3; i++)
            ts << " " << f[i];

        ts << std::endl;
    }

    for(auto&& page : pages) {
        if (page.pieces.size() > 0) {
            ts << "P " << nbPg++ << std::endl;
            for(auto&& piece : page.pieces) {
                ts << "p " << nbPi++ << " " << piece.O << std::endl;
                for(auto && fid : piece.faceId) {
                    Facette* f = getFacette(fid);
                    if (f->orig == -1) {
                        ts << "T " << f->id
                        << " " << f->triangle.a.x() << " " << f->triangle.a.y()
                        << " " << f->triangle.b.x() << " " << f->triangle.b.y()
                        << " " << f->triangle.c.x() << " " << f->triangle.c.y()
                        << std::endl;
                    } else {
                        ts << "t " << f->id << " " << f->orig << std::endl;
                    }
                }
            }
        }
    }
    ts << "L " << modeLanguettes << std::endl;
    if (modeLanguettes == 1) {
        for (auto&& l : flaps) {
            ts << "l " << l.fId << " " << l.nId << " " << l.state << std::endl;
        }
    }
}

int Unfold::read_pageId(std::string ch) {
    std::stringstream ss(ch);
    char T;
    int n;

    ss >> T >> n;
    return n;
}

QVector3D Unfold::read_points(std::string ch) {
    std::stringstream ss(ch);
    char T;
    qreal x, y, z;

    ss >> T >> x >> y >> z;
    return QVector3D(x, y, z);
}

std::vector<int> Unfold::read_facesDAT(std::string ch) {
    std::vector<int> r;
    std::stringstream ss(ch);
    std::string el;
    int f1, f2, f3, f4;
    char T;

    ss >> T >> f1 >> f2 >> f3 >> f4;
    r.push_back(f1);
    r.push_back(f2);
    r.push_back(f3);
    r.push_back(f4);

    return r;
}

std::vector<int> Unfold::read_faces(std::string ch) {
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
    return r;
}

std::string Unfold::read_group(std::string ch) {
    std::string r;
    std::stringstream ss(ch);
    char T;

    ss >> T >> r;

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
    ss.precision(2);

    char T;
    qreal x, y;
    ss >> T >> piece.id >> x >> y;
    piece.O.setX(x);
    piece.O.setY(y);

    return piece;
}

Facette Unfold::read_Premfacette(std::string ch){
    Facette f;
    std::stringstream ss(ch);
    ss.precision(2);

    char T;
    qreal  sax, say, sbx, sby, scx, scy;
    ss >> T >> f.id >> sax >> say >> sbx >> sby >> scx >> scy;
    f.orig = -1;
    f.triangle = Triangle2d(QPointF(sax, say), QPointF(sbx, sby), QPointF(scx, scy));

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
    int nb = 0;
    for(size_t nG = 0; nG < groups.size(); nG++) {
        Facette* prochainLibre;
        do {
            prochainLibre = FirstFreeFacette(static_cast<int>(nG));
            if(prochainLibre) {
                Page page = Page(static_cast<int>(pages.size()));
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
                                qreal angle = calc_angle(
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
                                    nb++;
                                    break;
                                }
                            }
                        }
                        if(ok)
                            break;
                    }
                }while(ok);

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
    QGraphicsView* vue, const QByteArray *donnees, qreal ech = 1.0) :
    fnOBJ(fOBJ), fnDAT(fDAT), fnSVG(fSVG), rVue(vue) {

    deja = false;
    optimiserNums = false;
    modeLanguettes = 0;
    hLanguettes = 15;
    if (!fOBJ.empty())
        load_OBJ(donnees);
    else if (!fDAT.empty())
        load_DAT(donnees);
    else {
        // erreur
        return;
    }
    init_triangles(ech);
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

            qreal angle = calc_angle(
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
            if (it->flags() & QGraphicsItem::ItemIsMovable) {
                //TitleItem* ti = static_cast<TitleItem*>(it);
                PieceItem* ti = static_cast<PieceItem*>(it);
                Piece* pi = getPiece(ti->data(0).toInt());
                if (pi) {
                    pi->O = QPointF(
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
    recalculeNums();

    bool doSVG = false;
    QFont fTitre, fNum;
    QPen pTitre, pNum;
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fTitre = QFont(fixedFont.family(), 15);
    pTitre = QPen(Qt::blue, 1);
    SVG::SVG root;
    QPointF se1, se2;

    if (!svg.isEmpty()) {
        doSVG = true;

        fNum = QFont(fixedFont.family(), 9);
        pNum = QPen(Qt::black, 1);

        // WA
        root.set_attr("width", "210mm");
        root.set_attr("height", "297mm");
        root.set_attr("viewBox", "0 0 533 754");

        // Basic CSS support
        root.style("path").set_attr("fill", "none").set_attr("stroke-width", "0.15mm");
    }

    QGraphicsScene* scene = new QGraphicsScene;
    flash = new QGraphicsLineItem;
    flash->setVisible(false);
    flash->setPen(QPen(Qt::green, 2));
    flash->setZValue(10);
    scene->addItem(flash);
    int dxP = 0;
    int fs = 0;
    int max_nO = 1;

    qreal lang_s = static_cast<float>(hLanguettes);
    qreal lang_dt = 0.45f;

    if (modeLanguettes == 2) {
        fs = 1;
    }

    SVG::Group *pageGroup;
    SVG::Path *pCoupes = nullptr, *pTextes = nullptr;
    SVG::Path *pPlisM = nullptr, *pPlisV = nullptr, *pPMS, *pPVS;

    for (auto&& page : pages) {
        scene->addRect(cmpo(2.5), cmpo(2.5), cmpo(pageDim.x()-5), cmpo(pageDim.y()-5))->setPos(cmpo(dxP), 0);
        pageGroup = nullptr;
        pCoupes = nullptr;
        pPlisM = nullptr;
        pPlisV = nullptr;
        pPMS = nullptr;
        pPVS = nullptr;
        pTextes = nullptr;

        bool pageOK = page.pieces.size() > 0;
        if (doSVG && pageOK) {
            pageGroup = root.add_child<SVG::Group>();
            QString snum = QString("_%1").arg(page.id+1, 3, 10, QChar('0'));
            pageGroup->set_attr("id", QString("page%1").arg(snum).toStdString());

            pTextes = pageGroup->add_child<SVG::Path>();
            pTextes->set_attr("id", QString("nums%1").arg(snum).toStdString());
            pTextes->set_attr("stroke", "black");

            pCoupes = pageGroup->add_child<SVG::Path>();
            pCoupes->set_attr("id", QString("coupes%1").arg(snum).toStdString());
            pCoupes->set_attr("stroke", "red");

            pPlisM = pageGroup->add_child<SVG::Path>();
            pPlisM->set_attr("id", QString("plisM%1").arg(snum).toStdString());
            pPlisM->set_attr("stroke", "brown");
            pPlisM->set_attr("stroke-dasharray", "4");

            pPMS = pageGroup->add_child<SVG::Path>();
            pPMS->set_attr("id", QString("marqueM%1").arg(snum).toStdString());
            pPMS->set_attr("stroke", "brown");

            pPlisV = pageGroup->add_child<SVG::Path>();
            pPlisV->set_attr("id", QString("plisV%1").arg(snum).toStdString());
            pPlisV->set_attr("stroke", "green");
            pPlisV->set_attr("stroke-dasharray", "4 1 1 1");

            pPVS = pageGroup->add_child<SVG::Path>();
            pPVS->set_attr("id", QString("marqueV%1").arg(snum).toStdString());
            pPVS->set_attr("stroke", "green");
        }

        for (auto&& piece : page.pieces) {
            std::vector<sedge> pedges = calculeLignes(piece);
            // construction du polygone depuis lLignes à partir de lLignes[0]
            QPolygonF qpoly = QPolygonF(PtsDepuisLignesDeCoupe(pedges));
            PieceItem *qp = new PieceItem(qpoly, piece.id, this);
            qp->setBrush(QBrush(calculeCouleurPiece(piece)));
            qp->setData(0, piece.id);
            qp->setData(1, qp->pos().x());
            qp->setData(2, qp->pos().y());
            scene->addItem(qp);

            //TitleItem *tit = new TitleItem(QString::number(piece.id+1), this);
            QGraphicsSimpleTextItem *tit = new QGraphicsSimpleTextItem(QString::number(piece.id+1));
            tit->setFont(fTitre);
            tit->setPen(pTitre);
            tit->setParentItem(qp);

            QPointF cTIT = calculeCentrePiece(piece);
            QPointF tt = cmpo(cTIT) + piece.O + QPointF(cmpo(5), cmpo(5));
            QPointF b = QPointF(tit->boundingRect().width(), tit->boundingRect().height()) / 2;
            cTIT = tt - b;
            tit->setPos(cTIT);
            tit->setZValue(5);
            if (doSVG && pageOK) {
                SVG::Path* tp = pageGroup->add_child<SVG::Path>();
                tp->set_attr("id", QString("titre_%1").arg(piece.id+1).toStdString());
                tp->set_attr("stroke", "indigo");
                drawHersheyInt(tp, tt + QPointF(0, 11*0.75), piece.id+1, 0, 0.75);
            }

            for (auto&& e : pedges) {
                if (e.nb == 1) {
                    Edge* ed = edgeGet(e.fid, e.nF);
                    if (ed->nO == -1)
                        ed->nO = max_nO++;
                }
                LineItem *li = new LineItem();
                li->setLine(QLineF(e.p1, e.p2));
                if (doSVG && pageOK) {
                    se1 = e.p1 + qp->pos();
                    se2 = e.p2 + qp->pos();
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
                        li->setOutlineColor((cop < 0) ? Qt::green : (cop > 0) ? QColor(180,0,0) : QColor(240,240,240));
                        li->setOutlineWidth(1);
                        li->setCustomPen();
                        if (doSVG && pageOK) {
                            if (li->pen().color() == Qt::green) {
                                drawLine(pPlisV, se1, se2);
                                drawX(pPVS, se1, se2);
                            } else if (li->pen().color() == QColor(180,0,0)) {
                                drawLine(pPlisM, se1, se2);
                                drawX(pPMS, se1, se2);
                            }
                        }

                        qreal d = distance(e.p1, e.p2);
                        qreal dt = lang_dt;
                        if (d > 50) dt = dt/2;
                        qreal a = degToRad(90) - direction(e.p1, e.p2);

                        QPointF P1 = rotatePt(e.p1 + QPointF(lang_s, d*dt), e.p1, a);
                        QPointF P2 = rotatePt(e.p1 + QPointF(lang_s, d*(1-dt)), e.p1, a);
                        QLineF LLang, LL1, LL2, l2;
                        QPointF iP{};
                        for(auto&& e2:pedges) {
                            LLang = QLineF(P1, P2);
                            LL1 = QLineF(e.p1, P1);
                            LL2 = QLineF(e.p2, P2);
                            l2 = QLineF(e2.p1, e2.p2);
                            if (LLang.intersects(l2, &iP) == QLineF::BoundedIntersection) {
                                if (distance(P1, iP) < distance(P2, iP)) {
                                    P1 = iP;
                                    P2 = e.p2;
                                } else {
                                    P2 = iP;
                                    P1 = e.p1;
                                }
                                LLang = QLineF(P1, P2);
                                LL1 = QLineF(e.p1, P1);
                                LL2 = QLineF(e.p2, P2);
                            }

                            if (!eq(e.p1, e2.p1) && !eq(e.p1, e2.p2) && LL1.intersects(l2, &iP) == QLineF::BoundedIntersection) {
                                P2 = iP;
                                LLang = QLineF(P1, P2);
                                LL2 = QLineF(e.p2, P2);
                            }

                            if (!eq(e.p2, e2.p1) && !eq(e.p2, e2.p2) && LL2.intersects(l2, &iP) == QLineF::BoundedIntersection) {
                                P1 = iP;
                                LLang = QLineF(P1, P2);
                                LL1 = QLineF(e.p1, P1);
                            }
                        }

                        QPainterPath path;
                        path.moveTo(e.p1);
                        if(!eq(e.p1, P1))
                            path.lineTo(P1);
                        path.lineTo(P2);
                        if(!eq(e.p2, P2))
                            path.lineTo(e.p2);
                        QGraphicsPathItem* pi = scene->addPath(path);
                        pi->setZValue(2);
                        pi->setParentItem(qp);
                        pi->setPen(QPen(Qt::red, 2));

                        if (doSVG && pageOK) {
                            svgPathMoveTo(pCoupes, se1);
                            if(!eq(e.p1, P1))
                                svgPathLineTo(pCoupes, P1 + qp->pos());
                            svgPathLineTo(pCoupes, P2 + qp->pos());
                            if(!eq(e.p2, P2))
                                svgPathLineTo(pCoupes, se2);
                        }
                    } else {
                        li->setTypeLigne(cop < 0 ? TL_COUPE_V : (cop > 0) ? TL_COUPE_M : TL_COUPE_C);
                        li->setOutlineWidth(2);
                        li->setCursor(QCursor(Qt::SizeHorCursor));
                        li->setOutlineColor(Qt::red);
                        li->setCustomPen();

                        if (doSVG && pageOK) {
                            drawLine(pCoupes, se1, se2);
                        }
                    }

                    Edge * edg = edgeGet(e.nF, e.fid);
                    if (edg) {
                        //int n = optimiserNums ? edg->nO : edg->nE;
                        int n = edg->nO;
                        NumItem *ti = new NumItem(QString::number(n), e.nF, e.fid);
                        QPointF b = QPointF(ti->boundingRect().width()/2, ti->boundingRect().height());
                        ti->setTransformOriginPoint(b);
                        qreal ra = angle(e.p1, e.p2) - std::numbers::pi;
                        qreal deg = qRadiansToDegrees(ra);
                        ti->setRotation(deg);
                        QPointF c = middle(e.p1, e.p2) - b;
                        ti->setPos(c);
                        ti->setZValue(2);
                        ti->setParentItem(qp);
                        if (doSVG && pageOK) {
                            drawHersheyInt(pTextes, c + b + qp->pos()
                            , n, 360-deg, 0.3);
                        }
                    }
                } else {
                    li->setLine(QLineF(e.p1, e.p2));
                    li->setTypeLigne(cop < 0 ? TL_LIE_V : (cop > 0) ? TL_LIE_M : TL_LIE_C);
                    li->setOutlineColor((cop < 0) ? Qt::green : (cop > 0) ? QColor(180,0,0) : QColor(240,240,240));
                    li->setOutlineWidth(1);
                    li->setCustomPen();
                    li->setCursor(QCursor(Qt::SplitHCursor));
                    li->setData(0, -1);
                    li->setData(1, e.fid);
                    li->setData(2, e.nF);
                    if (doSVG && pageOK) {
                        if (li->pen().color() == Qt::green) {
                            drawLine(pPlisV, se1, se2);
                            drawX(pPVS, se1, se2);
                        } else if (li->pen().color() == QColor(180,0,0)) {
                            drawLine(pPlisM, se1, se2);
                            drawX(pPMS, se1, se2);
                        }
                    }
                }
                li->setZValue(2);
                li->setParentItem(qp);
            }
            tit->setZValue(1);

        }
        dxP += pageDim.x();
    }
    if (doSVG) {
        svgRoot = QByteArray::fromStdString(std::string(root));
    }

    rVue->setScene(scene);
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
        //qInfo() << "STICK erreur : aretes d'une meme piece";
        return;
    }

    std::vector<fct> pool;
    std::vector<fct> newP;
    size_t maxP = 0;

  //std::cout << "FACETTES A DEPLACER" << std::endl;
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
  bool lok = false;
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
      //std::cout << "POOL" << std::endl;
      //for (auto&& ap : pool)
        //std::cout << (ap.pose ? "1" : "0") << " " << ap.id <<" " << ap.orig << std::endl;

  //std::cout << "RESULTAT" << std::endl;
  for (auto&& an : newP) {
    Facette *fn = getFacette(an.id);
    fn->orig = an.orig;
    fn->page = fA->page;
    fn->piece = fA->piece;
    //std::cout << (an.pose ? "1" : "0") << " " << an.id <<" " << an.orig << std::endl;
    Facette* facOrig = getFacette(an.orig);
    //std::cout << *fn << std::endl;
    Neighbor *n = facOrig->findNeighbor(an.id);
    fn->triangle += facOrig->triangle.point(n->id) - fn->triangle.point(n->idx);
    qreal angle = calc_angle(
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
    Facette* fA = getFacette(a);

    std::vector<fct> pool;
    std::vector<fct> newP;

    for (auto&& f : facettes) {
        if (f.page == fA->page) {
            fct fc;
            fc.id = f.id;
            fc.orig = f.orig;
            fc.pose = false;
            pool.push_back(fc);
        }
    }

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
        }
    } while (ok);

    int nPiece = pieceNextID();
    Piece* pieceA = getPiece(fA->piece);
    if (!pieceA) {
        //qInfo() << "SPLIT : erreur piece " << fA->piece;
        return;
    }
    Piece nP = Piece(nPiece);
    nP.O = QPointF(0,0);

    for (auto&& p : newP ) {
        Facette *pf = getFacette(p.id);
        if (!pf) {
            //qInfo() << "SPLIT : erreur facette " << p.id;
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

QPointF Unfold::calculeCentrePiece(Piece piece){
    QPointF cTIT{};
    for (auto&& fn : piece.faceId)
        cTIT += getFacette(fn)->triangle.centroid();
    cTIT /= piece.faceId.size();

    // recherche du centre de face le plus proche
    QPointF proche;
    qreal dist = 10000;
    for (auto&& fn : piece.faceId) {
        qreal d = distance(cTIT, getFacette(fn)->triangle.centroid());
        if (dist > d) {
            dist = d;
            proche = getFacette(fn)->triangle.centroid();
        }
    }
    return proche;
}

std::vector<sedge> Unfold::calculeLignes(Piece piece) {
    std::vector<sedge> pe;
    for (auto&& fn : piece.faceId) {
        Triangle2d tt = cmpo(getFacette(fn)->triangle) + piece.O + QPointF(cmpo(5), cmpo(5));
        for (int i = 0; i < 3 ; i++){
            // facette's edges and neighbors #
            int nF = getFacette(fn)->neighbors[i].nF;
            bool ok = true;
            QPointF p1 = tt.point(i),
                    p2 = tt.point(next(i));
            for (auto&& e: pe) {
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
                pe.push_back(edge);
            }
        }
    }
    return pe;
}

QColor Unfold::calculeCouleurPiece(Piece p) {
    QColor c;
    int ng = getFacette(p.faceId[0])->group;
    QString s = QString::fromStdString(groups[ng]);
    if (QColor::isValidColorName(s))
        c = QColor::fromString(s);
    else {
        QStringList sl = s.split("_", Qt::SkipEmptyParts);
        if (sl.size() >= 3) {
            bool ok0, ok1, ok2;
            int r, v, b;
            r = sl.at(sl.size() -3).toInt(&ok0);
            v = sl.at(sl.size() -2).toInt(&ok1);
            b = sl.at(sl.size() -1).toInt(&ok2);
            if(ok0 && ok1 && ok2) {
                c = QColor(r, v, b);
            }
            else
                c = gCOL[ng];
        }
        else
            c = gCOL[ng];
    }
    c.setAlpha(160);
    return c;
}

QList<QPointF> PtsDepuisLignesDeCoupe(std::vector<sedge> pe) {
    QList<QLineF> lLignes;
    for(auto&& e:pe){
        if (e.nb == 1)
            lLignes << QLineF(e.p1, e.p2);
    }

    QPointF premPoint = lLignes.first().p1();
    QPointF dernPoint = lLignes.first().p2();
    QList<QPointF> lPoints;
    lPoints << premPoint;
    lPoints << dernPoint;
    lLignes.removeFirst();

    while (lLignes.size() > 0) {
        for (QList<QLineF>::const_iterator i = lLignes.constBegin(); i != lLignes.constEnd(); i++) {
            QLineF l = *i;
            if (eq(l.p1(), premPoint)) {
                premPoint = l.p2();
                lPoints.prepend(premPoint);
                lLignes.erase(i);
                break;
            } else if (eq(l.p2(), premPoint)) {
                premPoint = l.p1();
                lPoints.prepend(premPoint);
                lLignes.erase(i);
                break;
            } else if (eq(l.p1(), dernPoint)) {
                dernPoint = l.p2();
                lPoints.append(dernPoint);
                lLignes.erase(i);
                break;
            } else if (eq(l.p2(), dernPoint)) {
                dernPoint = l.p1();
                lPoints.append(dernPoint);
                lLignes.erase(i);
                break;
            }
        }
    }
    lLignes.clear();
    return lPoints;
}
