#ifndef PIECE_H
#define PIECE_H

#include <vector>

#include <QVector2D>
#include "facette.h"
#include "triangle2d.h"

class Piece {
public :
    int id;
    QVector2D pMin;  // Biggest coordinates
    QVector2D pMax;  // Smallest coordinates
    QVector2D O;     // Origin
    //double a;   // rotation angle
    std::vector<int> faceId;

    Piece();
    Piece(int);

    void recadre(const Triangle2d&);
    void ajouteFace(Facette&, int, int, int);
    bool operator ==(const Piece&);
};


#endif // PIECE_H
