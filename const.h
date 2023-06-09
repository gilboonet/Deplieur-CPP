#ifndef CONST_H
#define CONST_H

#include "triangle2d.h"

struct sedge {
    QVector2D p1;
    QVector2D p2;
    int fid;
    int nid;
    int nF;
    int nb;
};

const float epsilon = 5;
const float dMin = -999999;
const float dMax =  999999;

int         prev(const int);
int         next(const int);
float       calc_angle(QVector2D, QVector2D, QVector2D);
float       radToDeg(float d);
float       degToRad(float d);
bool        eq(float a, float b);
bool        eq(QVector2D a, QVector2D b);
int         sgn(float);
float       cmpo(float);
QVector2D   cmpo(QVector2D v);
Triangle2d  cmpo(Triangle2d t);
QVector2D   middle (QVector2D, QVector2D);
float       distance(const QVector2D& v1, const QVector2D& v2);
float       angle(const QVector2D&, const QVector2D& p);
QVector2D   rotatePt(const QVector2D& v, const QVector2D& c, const float a);
float       direction (const QVector2D&, const QVector2D&);

#endif // CONST_H
