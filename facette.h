#ifndef FACETTE_H
#define FACETTE_H


#include <array>

#include "triangle2d.h"
#include "voisin.h"

class Facette { // Facette à déplier
public :
    int id;
    Triangle2d triangle;
    std::array<int,3 > faces;
    std::array<Voisin,3> voisins;
    int groupe;
    bool pose;
    int orig;
    int page;
    int piece;

    Facette ();

    bool overlap (const Facette &f);
};


#endif // FACETTE_H
