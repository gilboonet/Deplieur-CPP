#ifndef FACETTE_H
#define FACETTE_H


#include <array>

#include "triangle2d.h"
#include "neighbor.h"

class Facette { // Triangle to unfold
public :
    int id;
    Triangle2d triangle;
    std::array<int,3 > faces;
    std::array<Neighbor,3> neighbors;
    int group;
    bool pose;
    int orig;
    int page;
    int piece;

    Facette();

    bool overlap(const Facette&);
    Neighbor* findNeighbor(int);
};


#endif // FACETTE_H
