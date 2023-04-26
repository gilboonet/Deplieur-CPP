#include "triangle3d.h"
#include <cmath>
#include "const.h"

Triangle3d::Triangle3d() {}
Triangle3d::Triangle3d(Vec3 a, Vec3 b, Vec3 c) : a(a), b(b), c(c) {}

Vec3 Triangle3d::point(const int n) {
    return n == 0 ? this->a : n == 1 ? this->b : this->c;
}

Triangle2d Triangle3d::d2ize () {
    Vec3 d1 = this->b - this->a;
    Vec3 d2 = this->c - this->a;

    Vec2 P0 (0, 0);

    Vec2 P1 (
        sqrt((d1.x * d1.x) + (d1.y * d1.y) + (d1.z * d1.z)), 0);
    double P2x = ((d1.x * d2.x) + (d1.y * d2.y) + (d1.z * d2.z)) / P1.x;
    Vec2 P2 (
        P2x,
        sqrt((d2.x * d2.x) + (d2.y * d2.y) + (d2.z * d2.z) - (P2x * P2x))
    );

    return Triangle2d(P0, P1, P2);
}

bool Triangle3d::eq3 (Triangle3d t2, int n) {
    Vec3 pt = t2.point(n);

    bool r = pt.distance(this->a) >= epsilon
          && pt.distance(this->b) >= epsilon
          && pt.distance(this->c) >= epsilon;

    return r;
}

double Triangle3d::isCoplanar (Vec3 p) {
    Vec3 v1 = this->b - this->a;
    Vec3 v2 = this->c - this->a;

    double a = v1.y * v2.z - v2.y * v1.z;
    double b = v2.x * v1.z - v1.x * v2.z;
    double c = v1.x * v2.y - v1.y * v2.x;
    double d = -(a * this->a.x)
               -(b * this->a.y)
               -(c * this->a.z);

    return (a * p.x) + (b * p.y) + (c * p.z) + d;
}
