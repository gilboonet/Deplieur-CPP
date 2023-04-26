#include "vec3.h"
#include <cmath>

Vec3::Vec3() {}
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

double Vec3::distance (const Vec3 &v) {
    Vec3 d = Vec3 (v.x - this->x, v.y - this->y, v.z - this->z);
    return sqrt ((d.x * d.x) + (d.y * d.y) + (d.z * d.z));
}

Vec3 Vec3::operator+(const Vec3& v) {
    return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) {
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vec3 Vec3::operator * (const double& d) {
    return Vec3 (this->x * d, this->y * d, this->z * d);
}

Vec3 Vec3::operator / (const double& d) {
    return Vec3 (this->x / d, this->y / d, this->z / d);
}
