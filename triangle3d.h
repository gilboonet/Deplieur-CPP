#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include <QVector3D>
#include "triangle2d.h"

class Triangle3d
{
public:
    QVector3D a, b, c;

    Triangle3d();
    Triangle3d(QVector3D, QVector3D, QVector3D);

    QVector3D point(const int);
    Triangle2d d2ize();
    bool eq3(Triangle3d, int);
    qreal isCoplanar(QVector3D);
};


#endif // TRIANGLE3D_H
