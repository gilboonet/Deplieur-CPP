#ifndef UNFOLD_H
#define UNFOLD_H


#include <vector>
#include <array>
#include <string>
#include "vec3.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <ostream>

#include "vec2.h"
#include "vec3.h"
#include "triangle2d.h"
#include "triangle3d.h"
#include "neighbor.h"
#include "edge.h"
#include "copl.h"
#include "facette.h"
#include "const.h"
#include "piece.h"
#include "page.h"
#include "../svg.hpp"

class Unfold { // data and processes needed to unfold a volume
private :
    std::vector<Vec3>                   pts;
    std::vector<std::vector<int>>       faces;
    std::vector<std::string>            groups;
    std::vector<Triangle3d>             t3d;
    std::vector<Triangle2d>             t2d;
    std::vector<std::array<Neighbor, 3>> neighbors;
    std::vector<Edge>                   edges;
    std::vector<Copl>                   copl;
    int                                 nbFaces;
    std::vector<Facette>                facettes;
    std::vector<Page>                   pages;
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
    void    load_OBJ();
    Piece*  pieceGetById(std::vector<Piece> &, int);

public :
    Unfold(std::string, std::string, std::string);

    void    display_unfold(std::ostream &os);
    void    display_facettes(std::ostream&);
    void    load_DAT();
    static  Vec3             read_points(std::string);
    static  std::vector<int> read_faces(std::string, int);
    static  Page             read_page(std::string);
    static  Piece            read_piece(std::string);
    static  Facette         read_facette(std::string);
    void    init_unfolding();
    int     pieceNextID();
    void    unfolding();
    int     getNbFaces();
    void    display();
    void    create_SVG(std::string);
    void    reajuste_pieces();
};


#endif // UNFOLD_H
