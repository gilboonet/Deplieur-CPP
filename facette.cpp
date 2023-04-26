#include "facette.h"
#include "triangle2d.h"

Facette::Facette() {}

bool Facette::overlap(Facette f) {
    return this->triangle.overlap(f.triangle);
}
