#ifndef VEC2_H
#define VEC2_H
//#include "../svg.hpp"

class Vec2 {
public:
    double x, y;

    Vec2();
    Vec2(double, double);

    Vec2 operator +(const Vec2&);
    Vec2& operator+=(const Vec2&);
    Vec2 operator -(const Vec2&);
    Vec2 operator *(const double&);
    Vec2 operator /(const double&);
    bool operator ==(const Vec2&);
    bool operator <(const Vec2&) const;

    double distance(const Vec2&);
    double angle(const Vec2&);
    Vec2 rotate(const Vec2&, const double);
    Vec2 middle(const Vec2&);
    //SVG::Point SVG_Point();
};

#endif // VEC2_H
