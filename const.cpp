#include "const.h"
#include "vec2.h"
#include <cmath>

int prec (const int n) { // retourne le précédent dans le triplet (0,1,2)
    return n > 0 ? n - 1 : 2;
}

int suiv (const int n) {  // retourne le suivant dans le triplet (0,1,2)
    return n < 2 ? n + 1 : 0;
}

double calc_angle (Vec2 a, Vec2 b, Vec2 c) {
    Vec2 ab = b - a;
    Vec2 ac = c - a;

    double rot_ab_ac = atan2(ac.y * ab.x - ac.x * ab.y, ac.x * ab.x + ac.y * ab.y);
    return rot_ab_ac;
}

bool li (Vec2 l1S, Vec2 l1E, Vec2 l2S, Vec2 l2E) {
    // true if the lines intersect
    if ((l1S == l2S) || (l1S == l2E) || (l1E == l2S) || (l1E == l2E)) {
        return 0;
    }

    double denominator = ((l2E.y - l2S.y) * (l1E.x - l1S.x))
                         - ((l2E.x - l2S.x) * (l1E.y - l1S.y));

    if (denominator == 0)
        return 0;

    double
        a = l1S.y - l2S.y,
        b = l1S.x - l2S.x,
        numerator1 = ((l2E.x - l2S.x) * a) - ((l2E.y - l2S.y) * b),
        numerator2 = ((l1E.x - l1S.x) * a) - ((l1E.y - l1S.y) * b);
    a = numerator1 / denominator;
    b = numerator2 / denominator;

    if ((a > 0) && (a < 1) && (b > 0) && (b < 1))
        return 1;
    else
        return 0;
}
