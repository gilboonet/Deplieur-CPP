#include <cmath>

#include "const.h"
#include "vec2.h"

int prec(const int n) { // retourne le précédent dans le triplet(0,1,2)
    return n > 0 ? n - 1 : 2;
}

int suiv(const int n) {  // retourne le suivant dans le triplet(0,1,2)
    return n < 2 ? n + 1 : 0;
}

double calc_angle(Vec2 a, Vec2 b, Vec2 c) {
    Vec2 ab = b - a;
    Vec2 ac = c - a;

    double rot_ab_ac = atan2(ac.y * ab.x - ac.x * ab.y, ac.x * ab.x + ac.y * ab.y);
    return rot_ab_ac;
}
