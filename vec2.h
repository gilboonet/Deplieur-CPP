#ifndef VEC2_H
#define VEC2_H
#include <iostream>


class Vec2 {
public:
    double x, y;

    Vec2();
    Vec2(double x, double y);
    double distance (const Vec2 &v);
    double angle (const Vec2 &p);
    Vec2 rotation (const Vec2 &c, const double a);
    Vec2 operator+ (const Vec2& v);
    Vec2 operator- (const Vec2& v);
    Vec2 operator * (const double& d);
    Vec2 operator / (const double& d);
    bool operator == (const Vec2& v);
};

#endif // VEC2_H
