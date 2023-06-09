#include "triangle2d.h"
#include "const.h"

Triangle2d::Triangle2d() {}
Triangle2d::Triangle2d(QVector2D a, QVector2D b, QVector2D c) : a(a), b(b), c(c) {}

QVector2D Triangle2d::point(const int n) {
    return n == 0 ? this->a : n == 1 ? this->b : this->c;
}

Triangle2d Triangle2d::operator +(const QVector2D& v) {
    return Triangle2d(QVector2D(a + v), QVector2D(b + v), QVector2D(c + v));
}

Triangle2d& Triangle2d::operator +=(const QVector2D& v) {
    *this = Triangle2d(QVector2D(a + v), QVector2D(b + v), QVector2D(c + v));
    return *this;
}

Triangle2d Triangle2d::operator -(const QVector2D& v) {
    return Triangle2d(QVector2D(a - v), QVector2D(b - v), QVector2D(c - v));
}

Triangle2d& Triangle2d::operator -=(const QVector2D& v) {
    *this = Triangle2d(QVector2D(a - v), QVector2D(b - v), QVector2D(c - v));
    return *this;
}

Triangle2d Triangle2d::operator *(const float& v) {
    return Triangle2d(QVector2D(a * v), QVector2D(b * v), QVector2D(c * v));
}

Triangle2d Triangle2d::operator /(const float& v) {
    return Triangle2d(QVector2D(a / v), QVector2D(b / v), QVector2D(c / v));
}

Triangle2d& Triangle2d::rotate(const QVector2D& C, const float angle) {
    *this = Triangle2d(rotatePt(a, C, angle), rotatePt(b, C, angle), rotatePt(c, C, angle));
    return *this;
}

bool li(QVector2D l1S, QVector2D l1E, QVector2D l2S, QVector2D l2E) {
    // true if the lines intersect
    //if((l1S == l2S) ||(l1S == l2E) ||(l1E == l2S) ||(l1E == l2E)) {
    if (eq(l1S, l2S) || eq(l1S, l2E) || eq(l1E, l2S) || eq(l1E, l2E)) {
        return 0;
    }

    float denominator =((l2E.y() - l2S.y()) *(l1E.x() - l1S.x()))
                         -((l2E.x() - l2S.x()) *(l1E.y() - l1S.y()));

    if(denominator == 0)
        return 0;

    float
        a = l1S.y() - l2S.y(),
        b = l1S.x() - l2S.x(),
        numerator1 =((l2E.x() - l2S.x()) * a) -((l2E.y() - l2S.y()) * b),
        numerator2 =((l1E.x() - l1S.x()) * a) -((l1E.y() - l1S.y()) * b);
    a = numerator1 / denominator;
    b = numerator2 / denominator;

    if((a > 0) &&(a < 1) &&(b > 0) &&(b < 1))
        return 1;
    else
        return 0;
}

bool Triangle2d::overlap(const Triangle2d& t) {
    return li(this->a, this->b, t.a, t.b) || li(this->a, this->b, t.b, t.c) || li(this->a, this->b, t.c, t.a)
        || li(this->b, this->c, t.a, t.b) || li(this->b, this->c, t.b, t.c) || li(this->b, this->c, t.c, t.a)
        || li(this->c, this->a, t.a, t.b) || li(this->c, this->a, t.b, t.c) || li(this->c, this->a, t.c, t.a);
}

QVector2D Triangle2d::centroid() {
    return QVector2D(
       (a.x() + b.x() + c.x()) / 3,
       (a.y() + b.y() + c.y()) / 3
    );
}
