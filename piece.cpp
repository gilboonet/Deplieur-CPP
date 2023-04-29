#include "piece.h"
#include "const.cpp"

Piece::Piece() {}
Piece::Piece(int pid) :
    id(pid), pMin(Vec2(dMax, dMax)), pMax(Vec2(dMin, dMin)), O(Vec2(0.0, 0.0)) {}

void Piece::recadre(Triangle2d t) {
    for (int i = 0; i < 3; i++) {
        if (t.point(i).x < pMin.x) pMin.x = t.point(i).x;
        if (t.point(i).y < pMin.y) pMin.y = t.point(i).y;
        if (t.point(i).x > pMax.x) pMax.x = t.point(i).x;
        if (t.point(i).y > pMax.y) pMax.y = t.point(i).y;
    }
}

void Piece::ajouteFace(Facette* f, int orig) {
    (*f).orig = orig;
    faceId.push_back((*f).id);
    recadre((*f).triangle);
}
