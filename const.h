#ifndef CONST_H
#define CONST_H

#include "triangle2d.h"
#include "../svg.hpp"

struct sedge {
    QVector2D p1;
    QVector2D p2;
    int fid;
    int nid;
    int nF;
    int nb;
};

const double epsilon = 8;
const double dMin = -999999;
const double dMax =  999999;

int         prev(const int);
int         next(const int);
double      calc_angle(QVector2D, QVector2D, QVector2D);
double      radToDeg(double d);
double      degToRad(double d);
bool        eq(double a, double b);
bool        eq(QVector2D a, QVector2D b);
int         sgn(double);
double      cmpo(double);
QVector2D   cmpo(QVector2D v);
Triangle2d  cmpo(Triangle2d t);
SVG::Point  SVG_Point(QVector2D);
QVector2D   middle (QVector2D, QVector2D);
double      distance(const QVector2D& v1, const QVector2D& v2);
double      angle(const QVector2D&, const QVector2D& p);
QVector2D   rotatePt(const QVector2D& v, const QVector2D& c, const double a);
double      direction (const QVector2D&, const QVector2D&);

#endif // CONST_H
