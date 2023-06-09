#include <cmath>
#include <numbers>

#include "const.h"

int prev(const int n) { // returns previous value from triplet(0,1,2)
    return n > 0 ? n - 1 : 2;
}

int next(const int n) {  // returns next value from triplet(0,1,2)
    return n < 2 ? n + 1 : 0;
}

bool eq(float a, float b) {
    return std::abs(a - b) < epsilon;
}

bool eq(QVector2D a, QVector2D b) {
    return eq(a.x(), b.x()) && eq(a.y(), b.y());
}

float radToDeg(float r) {
    return (r * 180) / std::numbers::pi;
}

float degToRad(float d) {
    return d * std::numbers::pi / 180;
}

float calc_angle(QVector2D a, QVector2D b, QVector2D c) {
    QVector2D ab = b - a;
    QVector2D ac = c - a;

    float rot_ab_ac = atan2(ac.y() * ab.x() - ac.x() * ab.y(), ac.x() * ab.x() + ac.y() * ab.y());
    return rot_ab_ac;
}

int sgn(float x) {
   if (x < 0) return 1;
   if (x > 0) return -1;
   return 0;
}

float cmpo(float x) { return x * 2.54;}

QVector2D cmpo(QVector2D v) { return v * 2.54;}

Triangle2d cmpo(Triangle2d t) { return t * 2.54;}

QVector2D middle (QVector2D a, QVector2D b) {
    return (a + b) / 2;
}

float distance(const QVector2D& v1, const QVector2D& v2) {
    QVector2D d = v2 - v1;
    return sqrt((d.x() * d.x()) +(d.y() * d.y()));
}

float angle(const QVector2D& v, const QVector2D& p) {
    return atan2(p.y() - v.y(), p.x() - v.x());
}

QVector2D rotatePt(const QVector2D& v, const QVector2D& c, const float a) {
    float lcos = cos(a);
    float lsin = sin(a);

    QVector2D r = QVector2D(
       (lcos *(v.x() - c.x())) +(lsin *(v.y() - c.y())) + c.x(),
       (lcos *(v.y() - c.y())) -(lsin *(v.x() - c.x())) + c.y()
    );
    return r;
}

float direction(const QVector2D& p1, const QVector2D& p2) {
    return atan2(p2.y() - p1.y(), p2.x() - p1.x());
}
