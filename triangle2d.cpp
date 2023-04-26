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

/*Triangle2d Triangle2d::translate(const Vec2& v) {
    return Triangle2d(Vec2(a + v), Vec2(b + v), Vec2(c + v)); 
}*/

Triangle2d Triangle2d::rotation(const Vec2& C, const double angle) {
    return Triangle2d(a.rotation(C, angle), b.rotation(C, angle), c.rotation(C, angle));
}

bool Triangle2d::overlap(const Triangle2d& t) {
    return li(this->a, this->b, t.a, t.b) || li(this->a, this->b, t.b, t.c) || li(this->a, this->b, t.c, t.a)
        || li(this->b, this->c, t.a, t.b) || li(this->b, this->c, t.b, t.c) || li(this->b, this->c, t.c, t.a)
        || li(this->c, this->a, t.a, t.b) || li(this->c, this->a, t.b, t.c) || li(this->c, this->a, t.c, t.a);
}
