#ifndef FACETTE_H
#define FACETTE_H
#include "triangle2d.h"
#include "voisin.h"
#include <array>

using std::array;

class Facette { // Facette à déplier
public :
    int id;
    Triangle2d triangle;
    array<int,3 > faces;
    array<Voisin,3> voisins;
    int groupe;
    bool pose;
    int orig;

    Facette();
    bool overlap(Facette f);
};


#endif // FACETTE_H
