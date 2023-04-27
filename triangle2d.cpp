#include "triangle2d.h"
#include "const.h"

Triangle2d::Triangle2d() {}
Triangle2d::Triangle2d(Vec2 a, Vec2 b, Vec2 c) : a(a), b(b), c(c) {}

Vec2 Triangle2d::point(const int n) {
    return n == 0 ? this->a : n == 1 ? this->b : this->c;
}

Triangle2d Triangle2d::operator + (const Vec2& v) {
    return Triangle2d(Vec2(a + v), Vec2(b + v), Vec2(c + v));
}

Triangle2d Triangle2d::operator - (const Vec2& v) {
    return Triangle2d(Vec2(a - v), Vec2(b - v), Vec2(c - v));
}

Triangle2d Triangle2d::operator * (const double& v) {
    return Triangle2d(Vec2(a * v), Vec2(b * v), Vec2(c * v));
}

Triangle2d Triangle2d::operator / (const double& v) {
    return Triangle2d(Vec2(a / v), Vec2(b / v), Vec2(c / v));
}

Triangle2d Triangle2d::rotation(const Vec2& C, const double angle) {
    return Triangle2d(a.rotation(C, angle), b.rotation(C, angle), c.rotation(C, angle));
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

bool Triangle2d::overlap(const Triangle2d& t) {
    return li(this->a, this->b, t.a, t.b) || li(this->a, this->b, t.b, t.c) || li(this->a, this->b, t.c, t.a)
        || li(this->b, this->c, t.a, t.b) || li(this->b, this->c, t.b, t.c) || li(this->b, this->c, t.c, t.a)
        || li(this->c, this->a, t.a, t.b) || li(this->c, this->a, t.b, t.c) || li(this->c, this->a, t.c, t.a);
}
