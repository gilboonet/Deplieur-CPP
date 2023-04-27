#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H


#include "vec2.h"

class Triangle2d
{
public:
    Vec2 a, b, c;

    Triangle2d ();
    Triangle2d (Vec2 a, Vec2 b, Vec2 c);

    Triangle2d operator + (const Vec2& v);
    Triangle2d operator - (const Vec2& v);
    Triangle2d operator * (const double& v);
    Triangle2d operator / (const double& v);

    Vec2 point (const int n);
    Triangle2d rotation (const Vec2& C, const double angle);
    bool overlap (const Triangle2d& t2);
};

bool li (Vec2 l1S, Vec2 l1E, Vec2 l2S, Vec2 l2E);


#endif // TRIANGLE2D_H
