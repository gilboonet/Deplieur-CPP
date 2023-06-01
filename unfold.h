#ifndef UNFOLD_H
#define UNFOLD_H

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <numbers>
#include <ostream>
#include <functional>
#include <QString>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>

#include <QVector3D>
#include <QVector2D>
#include "triangle2d.h"
#include "triangle3d.h"
#include "neighbor.h"
#include "edge.h"
#include "copl.h"
#include "facette.h"
#include "const.h"
#include "piece.h"
#include "page.h"
//#include "titleitem.h"
#include "../svg.hpp"

class TitleItem;

enum LINE_EVT { LE_CUT, LE_LNK };

typedef struct
{
    LINE_EVT type;
    int a;
    int b;
} le_data;

Q_DECLARE_METATYPE(le_data);

class Unfold { // data and processes needed to unfold a volume
private :
    std::vector<QVector3D>              pts;
    std::vector<std::vector<int>>       faces;
    std::vector<std::string>            groups;
    std::vector<Triangle3d>             t3d;
    std::vector<Triangle2d>             t2d;
    std::vector<std::array<Neighbor, 3>> neighbors;
    std::vector<Edge>                   edges;
    std::vector<Copl>                   copl;
    int                                 nbFaces;
    std::vector<Facette>                facettes;
    std::string                         fnOBJ;
    std::string                         fnDAT;
    std::string                         fnSVG;

    Facette* FirstFreeFacette(int);
    void    init_triangles();
    void    calc_neighbourhood();
    void    calc_copl();
    void    num_edges();
    void    display_points();
    void    display_triangles();
    void    display_faces();
    void    display_groups();
    void    display_edges();
    void    display_neighbourhood();
    void    display_copl();
    void    load_OBJ();
    Piece*  pieceGetById(std::vector<Piece> &, int);
    Edge*   edgeGet(int fid, int nid);

public :
    Unfold();
    Unfold(std::string, std::string, std::string, QGraphicsView*, QSlider*);

    std::vector<Page>       pages;
    QGraphicsView*          rVue;
    QSlider*                rSlider;
    bool                    deja;
    TitleItem*              titleItem;
    int                     IdPieceCourante;
    void                    display_unfold(std::ostream &);
    void                    display_facettes(std::ostream&);
    void                    load_DAT();
    static  QVector3D       read_points(std::string);
    static  std::vector<int> read_faces(std::string, int);
    static  Page            read_page(std::string);
    static  Piece           read_piece(std::string);
    static  Facette         read_facette(std::string);
    void                    init_unfolding();
    int                     pieceNextID();
    void                    unfolding();
    int                     getNbFaces();
    void                    display();
    void                    create_SVG(std::string);
    void                    create_SVG(QString);
    void                    displayUI();
    void                    reajuste_pieces();
    Copl*                   getCopl(int, int);
    Facette*                getFacette(const int);
    Piece*                  getPieceCourante();
    void                    setPieceCourante(const int, TitleItem*);
    Page*                   getPage(const int);
    Piece*                  getPiece(const int);
    void                    rotatePieceCourante(int, int = 0);
    void                    deplacePieceCourante(int, int);
    void                    stickPiece(int, int);
    void                    splitPiece(int, int);
};

std::ostream& operator <<(std::ostream& os, const QVector2D& v);
std::ostream& operator <<(std::ostream& os, const QVector3D& v);
std::ostream& operator <<(std::ostream& os, const Triangle2d& t);
std::ostream& operator <<(std::ostream& os, const Triangle3d& t);


#endif // UNFOLD_H
