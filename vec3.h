#ifndef VEC3_H
#define VEC3_H


class Vec3 {
public:
    double x, y, z;

    Vec3 ();
    Vec3 (double x, double y, double z);

    Vec3 operator + (const Vec3& v);
    Vec3 operator - (const Vec3& v);
    Vec3 operator * (const double& d);
    Vec3 operator / (const double& d);

    double distance (const Vec3& v);
};


#endif // VEC3_H
