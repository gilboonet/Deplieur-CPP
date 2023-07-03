#ifndef CONST_H
#define CONST_H

#include "../svg.hpp"
#include "triangle2d.h"

struct sedge {
    QPointF p1;
    QPointF p2;
    int fid;
    int nid;
    int nF;
    int nb;
};

const qreal epsilon = 1;
const qreal dMin = -999999;
const qreal dMax =  999999;

int         prev(const int);
int         next(const int);
qreal       calc_angle(QPointF, QPointF, QPointF);
qreal       radToDeg(qreal);
qreal       degToRad(qreal);
bool        eq(qreal, qreal);
bool        eq(QPointF, QPointF);
int         sgn(qreal);
qreal       cmpo(qreal);
QPointF     cmpo(QPointF);
Triangle2d  cmpo(Triangle2d);
QPointF     middle (QPointF, QPointF);
qreal       distance(const QPointF&, const QPointF&);
qreal       angle(const QPointF&, const QPointF&);
QPointF     rotatePt(const QPointF&, const QPointF&, const qreal);
qreal       direction (const QPointF&, const QPointF&);

void        svgPathMoveTo(SVG::Path*, QPointF);
void        svgPathLineTo(SVG::Path*, QPointF);
void        drawLine(SVG::Path *, QPointF, QPointF);
void        drawX(SVG::Path *, QPointF, QPointF);
void        drawHersheyInt(SVG::Path *, QPointF, int, qreal, qreal);


#endif // CONST_H
