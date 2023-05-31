#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H


#include <QVector2D>

class Triangle2d
{
public:
    QVector2D a, b, c;

    Triangle2d();
    Triangle2d(QVector2D, QVector2D, QVector2D);

    Triangle2d operator +(const QVector2D&);
    Triangle2d &operator +=(const QVector2D&);
    Triangle2d operator -(const QVector2D&);
    Triangle2d &operator -=(const QVector2D&);
    Triangle2d operator *(const double&);
    Triangle2d operator /(const double&);

    QVector2D point(const int);
    Triangle2d& rotate(const QVector2D&, const double);
    bool overlap(const Triangle2d&);
    QVector2D centroid();
};

bool li(QVector2D, QVector2D, QVector2D, QVector2D);


#endif // TRIANGLE2D_H
