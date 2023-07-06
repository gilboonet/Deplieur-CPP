#ifndef CONST_H
#define CONST_H

#include "../svg.hpp"
#include "triangle2d.h"
#include <QColor>

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

const QColor gCOL[] = {
    QColor::fromString("blue"),
    QColor::fromString("green"),
    QColor::fromString("yellow"),
    QColor::fromString("orange"),
    QColor::fromString("cyan"),
    QColor::fromString("magenta"),
    QColor::fromString("gray"),

    QColor::fromString("darkRed"),
    QColor::fromString("darkGreen"),
    QColor::fromString("darkBlue"),
    QColor::fromString("darkCyan"),
    QColor::fromString("darkMagenta"),
    QColor::fromString("darkYellow"),
    QColor::fromString("darkGray"),

    QColor::fromString("aliceblue"),
    QColor::fromString("antiquewhite"),
    QColor::fromString("aqua"),
    QColor::fromString("aquamarine"),
    QColor::fromString("azure"),
    QColor::fromString("beige"),
    QColor::fromString("bisque"),
    QColor::fromString("blanchedalmond"),
    QColor::fromString("blueviolet"),
    QColor::fromString("brown"),
    QColor::fromString("burlywood"),
    QColor::fromString("cadetblue"),
    QColor::fromString("chartreuse"),
    QColor::fromString("chocolate"),
    QColor::fromString("coral"),
    QColor::fromString("cornflowerblue"),
    QColor::fromString("cornsilk"),
    QColor::fromString("crimson"),
    QColor::fromString("darkgoldenrod"),
    QColor::fromString("darkkhaki"),
};

#endif // CONST_H
