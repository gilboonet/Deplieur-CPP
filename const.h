#ifndef CONST_H
#define CONST_H

#include "../svg.hpp"
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
float       radToDeg(float);
float       degToRad(float);
bool        eq(float, float);
bool        eq(QVector2D, QVector2D);
int         sgn(float);
float       cmpo(float);
QVector2D   cmpo(QVector2D);
Triangle2d  cmpo(Triangle2d);
QVector2D   middle (QVector2D, QVector2D);
float       distance(const QVector2D&, const QVector2D&);
float       angle(const QVector2D&, const QVector2D&);
QVector2D   rotatePt(const QVector2D&, const QVector2D&, const float);
QPointF     rotatePt(const QPointF&, const QPointF&, const float);
float       direction (const QVector2D&, const QVector2D&);

void        svgPathMoveTo(SVG::Path*, QPointF);
void        svgPathLineTo(SVG::Path*, QPointF);
void        drawHersheyInt(SVG::Path *, QPointF, int, double, double);


#endif // CONST_H
