#ifndef PIECE_H
#define PIECE_H
#include "vec2.h"
#include "facette.h"
#include "triangle2d.h"
#include <vector>

using std::vector;

class Piece {
public :
    int id;
    Vec2 pMin;
    Vec2 pMax;
    vector<Facette> facettes;

    Piece();
    Piece(int pid);

    void recadre(Triangle2d t);

    void ajouteFace(Facette f, int orig);
};

#endif // PIECE_H
