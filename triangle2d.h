#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H


#include "vec2.h"

class Triangle2d
{
public:
    Vec2 a, b, c;

    Triangle2d();
    Triangle2d(Vec2, Vec2, Vec2);

    Triangle2d operator +(const Vec2&);
    Triangle2d &operator +=(const Vec2&);
    Triangle2d operator -(const Vec2&);
    Triangle2d &operator -=(const Vec2&);
    Triangle2d operator *(const double&);
    Triangle2d operator /(const double&);

    Vec2 point(const int);
    Triangle2d& rotate(const Vec2&, const double);
    bool overlap(const Triangle2d&);
    Vec2 centroid();
};

bool li(Vec2, Vec2, Vec2, Vec2);


#endif // TRIANGLE2D_H
