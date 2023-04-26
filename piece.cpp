#include "piece.h"
#include "const.cpp"

Piece::Piece() {}
Piece::Piece(int pid) :
    id(pid), pMin(Vec2(dMax, dMax)), pMax(Vec2(dMin, dMin))
{}

void Piece::recadre(Triangle2d t) {
    for (int i = 0; i < 3; i++) {
        if (t.point(i).x < pMin.x) pMin.x = t.point(i).x;
        if (t.point(i).y < pMin.y) pMin.y = t.point(i).y;
        if (t.point(i).x > pMax.x) pMax.x = t.point(i).x;
        if (t.point(i).y > pMax.y) pMax.y = t.point(i).y;
    }
}

void Piece::ajouteFace(Facette f, int orig) {
    f.orig = orig;
    facettes.push_back(Facette(f));
    recadre(f.triangle);
}

