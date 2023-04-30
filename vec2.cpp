#include "vec2.h"
#include <cmath>
#include "const.h"

Vec2::Vec2() {}

Vec2::Vec2(double x, double y) : x(x), y(y) {}

double Vec2::distance(const Vec2& v) {
    Vec2 d = Vec2(v.x - this->x, v.y - this->y);
    return sqrt((d.x * d.x) +(d.y * d.y));
}

double Vec2::angle(const Vec2& p) {
    return atan2(p.y - this->y, p.x - this->x);
}

Vec2 Vec2::rotation(const Vec2& c, const double a) {
    double lcos = cos(a);
    double lsin = sin(a);

    Vec2 r = Vec2(
       (lcos *(this->x - c.x)) +(lsin *(this->y - c.y)) + c.x,
       (lcos *(this->y - c.y)) -(lsin *(this->x - c.x)) + c.y
    );

    return r;

}

bool Vec2::operator ==(const Vec2& v) {
    return this->distance(v) < epsilon;
}

Vec2 Vec2::operator+(const Vec2& v) {
    return Vec2(this->x + v.x, this->y + v.y);
}

Vec2 Vec2::operator-(const Vec2& v) {
    return Vec2(this->x - v.x, this->y - v.y);
}

Vec2 Vec2::operator *(const double& d) {
    return Vec2(this->x * d, this->y * d);
}

Vec2 Vec2::operator /(const double& d) {
    return Vec2(this->x / d, this->y / d);
}
