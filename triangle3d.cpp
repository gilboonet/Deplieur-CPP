#include "triangle3d.h"
#include <cmath>
#include "const.h"

Triangle3d::Triangle3d() {}
Triangle3d::Triangle3d(QVector3D a, QVector3D b, QVector3D c) : a(a), b(b), c(c) {}

QVector3D Triangle3d::point(const int n) {
    return n == 0 ? this->a : n == 1 ? this->b : this->c;
}

Triangle2d Triangle3d::d2ize() {
    QVector3D d1 = this->b - this->a;
    QVector3D d2 = this->c - this->a;

    QVector2D P0(0, 0);

    QVector2D P1(sqrt((d1.x() * d1.x()) +(d1.y() * d1.y()) +(d1.z() * d1.z())), 0);
    double P2x =((d1.x() * d2.x()) +(d1.y() * d2.y()) +(d1.z() * d2.z())) / P1.x();
    QVector2D P2(
        P2x,
        sqrt((d2.x() * d2.x()) +(d2.y() * d2.y()) +(d2.z() * d2.z()) -(P2x * P2x))
    );

    return Triangle2d(P0, P1, P2);
}

bool Triangle3d::eq3(Triangle3d t2, int n) {
    QVector3D pt = t2.point(n);

    bool r = pt.distanceToPoint(this->a) >= epsilon
          && pt.distanceToPoint(this->b) >= epsilon
          && pt.distanceToPoint(this->c) >= epsilon;

    return r;
}

double Triangle3d::isCoplanar(QVector3D p) {
    QVector3D v1 = this->b - this->a;
    QVector3D v2 = this->c - this->a;


    double a = v1.y() * v2.z() - v2.y() * v1.z();
    double b = v2.x() * v1.z() - v1.x() * v2.z();
    double c = v1.x() * v2.y() - v1.y() * v2.x();
    double d = -(a * this->a.x())
               -(b * this->a.y())
               -(c * this->a.z());

    return(a * p.x()) +(b * p.y()) +(c * p.z()) + d;
}
