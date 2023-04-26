#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H
#include "vec3.h"
#include "triangle2d.h"

class Triangle3d
{
public:
    Vec3 a, b, c;

    Triangle3d ();

    Triangle3d (Vec3 a, Vec3 b, Vec3 c);

    Vec3 point (const int n);

    Triangle2d d2ize ();

    bool eq3 (Triangle3d t2, int n);

    double isCoplanar (Vec3 p);
};



#endif // TRIANGLE3D_H
