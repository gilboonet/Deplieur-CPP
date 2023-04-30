#ifndef VEC3_H
#define VEC3_H


class Vec3 {
public:
    double x, y, z;

    Vec3();
    Vec3(double, double, double);

    Vec3 operator +(const Vec3&);
    Vec3 operator -(const Vec3&);
    Vec3 operator *(const double&);
    Vec3 operator /(const double&);

    double distance(const Vec3&);
};


#endif // VEC3_H
