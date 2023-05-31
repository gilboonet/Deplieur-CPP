#include "piece.h"
#include "const.cpp"

Piece::Piece() {}
Piece::Piece(int pid) :
    id(pid), pMin(QVector2D(dMax, dMax)), pMax(QVector2D(dMin, dMin)), O(QVector2D(0.0, 0.0)), a(0.0) {}

void Piece::recadre(const Triangle2d& tr) {
    Triangle2d t = tr;
    for(int i = 0; i < 3; i++) {
        if(t.point(i).x() < pMin.x()) pMin.setX(t.point(i).x());
        if(t.point(i).y() < pMin.y()) pMin.setY(t.point(i).y());
        if(t.point(i).x() > pMax.x()) pMax.setX(t.point(i).x());
        if(t.point(i).y() > pMax.y()) pMax.setY(t.point(i).y());
    }
}

void Piece::ajouteFace(Facette& fa, int orig, int page, int piece) {
    fa.orig = orig;
    this->faceId.push_back(fa.id);
    recadre(fa.triangle);
    fa.page = page;
    fa.piece = piece;
    fa.pose = true;
}

bool Piece::operator ==(const Piece& p) {
    if (this->id == p.id)
        return true;
    return false;
};
