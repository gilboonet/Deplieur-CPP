#ifndef PIECE_H
#define PIECE_H

#include <vector>

#include "vec2.h"
#include "facette.h"
#include "triangle2d.h"

class Piece {
public :
    int id;
    Vec2 pMin;  // Biggest coordinates
    Vec2 pMax;  // Smallest coordinates
    Vec2 O;     // Origin
    double a;   // rotation angle
    std::vector<int> faceId;

    Piece();
    Piece(int);

    void recadre(const Triangle2d&);
    void ajouteFace(Facette&, int, int, int);
};


#endif // PIECE_H
