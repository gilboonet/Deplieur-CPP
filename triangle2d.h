#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H
#include "vec2.h"

class Triangle2d
{
public:
    Vec2 a, b, c;

    Triangle2d ();
    Triangle2d (Vec2 a, Vec2 b, Vec2 c);

    Vec2 point (const int n);

    //Triangle2d translate (const Vec2& v);
    Triangle2d rotation (const Vec2& C, const double angle);
    Triangle2d operator + (const Vec2& v);
    Triangle2d operator - (const Vec2& v);
    Triangle2d operator * (const double& v);
    Triangle2d operator / (const double& v);

    bool overlap (const Triangle2d& t2);
};

#endif // TRIANGLE2D_H
