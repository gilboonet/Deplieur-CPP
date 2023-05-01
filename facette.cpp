#include <iostream>
#include "facette.h"
#include "triangle2d.h"

Facette::Facette() {}

bool Facette::overlap(const Facette& f) {
    return this->triangle.overlap(f.triangle);
}

Neighbor* Facette::findNeighbor(int nId) {
    for(auto&& n : this->neighbors) {
        if (n.nF == nId)
            return &n;
    }
    return nullptr;
}
