#include <cmath>
#include <numbers>
#include <QtMath>
#include <QString>

#include <QDebug>

#include "const.h"

int prev(const int n) { // returns previous value from triplet(0,1,2)
    return n > 0 ? n - 1 : 2;
}

int next(const int n) {  // returns next value from triplet(0,1,2)
    return n < 2 ? n + 1 : 0;
}

bool eq(qreal a, qreal b) {
    return qFabs(a - b) < epsilon;
}

bool eq(QPointF a, QPointF b) {
    return eq(a.x(), b.x()) && eq(a.y(), b.y());
}

QPointF plusPetit(QPointF a, QPointF b) {
    if ((a.x() < b.x()) || (eq(a.x(), b.x()) && (a.y() < b.y())) )
        return a;
    else
        return b;
}

qreal radToDeg(qreal r) {
    return (r * 180) / std::numbers::pi;
}

qreal degToRad(qreal d) {
    return d * std::numbers::pi / 180;
}

// returns square of distance b/w two points0
int CarreDistance(QPointF a, QPointF b)
{
    qreal xDiff = a.x() - b.x();
    qreal yDiff = a.y() - b.y();

    int r = static_cast<int> (xDiff*xDiff + yDiff*yDiff);
    return r;
}
void calcAngles(QPointF A, QPointF B, QPointF C)
{
    // Square of lengths be a2, b2, c2
    int a2 = CarreDistance(B, C);
    int b2 = CarreDistance(A, C);
    int c2 = CarreDistance(A, B);

    // length of sides be a, b, c
    qreal a = sqrt(a2);
    qreal b = sqrt(b2);
    qreal c = sqrt(c2);

    // From Cosine law
    qreal alpha = acos((b2 + c2 - a2)/(2*b*c));
    qreal beta = acos((a2 + c2 - b2)/(2*a*c));
    qreal gamma = acos((a2 + b2 - c2)/(2*a*b));

    // Converting to degree
    alpha = alpha * 180 / PI;
    beta = beta * 180 / PI;
    gamma = gamma * 180 / PI;

    // printing all the angles
    qDebug() << "alpha : " << alpha << "beta : " << beta << "gamma : " << gamma;
}

int getAngleABC( QPointF a, QPointF b, QPointF c)
{
    QPointF ab = b - a;
    QPointF cb = b - c;

    qreal dot = (ab.x() * cb.x() + ab.y() * cb.y()); // dot product
    qreal cross = (ab.x() * cb.y() - ab.y() * cb.x()); // cross product

    qreal alpha = atan2(cross, dot);

    int r = static_cast<int>(radToDeg(floor(alpha * 180. / std::numbers::pi + 0.5)));
    r = r % 360;
    return r;
}

qreal calc_angle(QPointF a, QPointF b, QPointF c) {
    QPointF ab = b - a;
    QPointF ac = c - a;

    qreal rot_ab_ac = atan2(ac.y() * ab.x() - ac.x() * ab.y(), ac.x() * ab.x() + ac.y() * ab.y());
    return rot_ab_ac;
}

int sgn(qreal x) {
   if (x < 0) return 1;
   if (x > 0) return -1;
   return 0;
}

qreal cmpo(qreal x) { return x * 2.54;}

QPointF cmpo(QPointF v) { return v * 2.54;}

Triangle2d cmpo(Triangle2d t) { return t * 2.54;}

QPointF middle (QPointF a, QPointF b) {
    return (a + b) / 2;
}

qreal distance(const QPointF& v1, const QPointF& v2) {
    QPointF d = v2 - v1;
    return sqrt((d.x() * d.x()) +(d.y() * d.y()));
}

qreal angle(const QPointF& v, const QPointF& p) {
    return atan2(p.y() - v.y(), p.x() - v.x());
}

QPointF rotatePt(const QPointF& v, const QPointF& c, const qreal a) {
    qreal lcos = cos(a);
    qreal lsin = sin(a);

    QPointF r = QPointF(
       (lcos *(v.x() - c.x())) +(lsin *(v.y() - c.y())) + c.x(),
       (lcos *(v.y() - c.y())) -(lsin *(v.x() - c.x())) + c.y()
    );
    return r;
}

qreal direction(const QPointF& p1, const QPointF& p2) {
    return atan2(p2.y() - p1.y(), p2.x() - p1.x());
}

void svgPathMoveTo(SVG::Path* path, QPointF point) {
    path->move_to(point.x(), point.y());
}

void svgPathLineTo(SVG::Path* path, QPointF point) {
    path->line_to(point.x(), point.y());
}

void drawLine(SVG::Path *path, QPointF p1, QPointF p2) {
    svgPathMoveTo(path, p1);
    svgPathLineTo(path, p2);
}

void drawX(SVG::Path *path, QPointF p1, QPointF p2) {
    QPointF c = middle(p1, p2);
    path->move_to(c.x() -2, c.y()-2);
    path->line_to(c.x() +2, c.y()+2);
    path->move_to(c.x() +2, c.y()-2);
    path->line_to(c.x() -2, c.y()+2);
}

void drawHersheyInt(SVG::Path *path, QPointF orig, int val, qreal angle, qreal ech = 1) {

struct spi {
    bool is_move;
    QPointF pt;
};

    std::vector<spi> lspi;
    QString s = QString::number(val);
    QPointF O{};
    qreal maxX = 0;
    for (const auto c : s) {
        int num = c.digitValue();
        std::string p =
          num == 0 ? "M 4.875 0.5 L 2.625 1.5 L 1.125 4.5 L 0.375 9.5 L 0.375 12.5 L 1.125 17.5 L 2.625 20.5 L 4.875 21.5 L 6.375 21.5 L 8.625 20.5 L 10.125 17.5 L 10.875 12.5 L 10.875 9.5 L 10.125 4.5 L 8.625 1.5 L 6.375 0.5 L 5.25 0.5"
        : num == 1 ? "M 0.15 5.2 L 1.65 4.2 L 3.9 1.2 L 3.9 22.2"
        : num == 2 ? "M 1.65 5.5 L 1.65 4.5 L 2.4 2.5 L 3.15 1.5 L 4.65 0.5 L 7.65 0.5 L 9.15 1.5 L 9.9 2.5 L 10.65 4.5 L 10.65 6.5 L 9.9 8.5 L 8.4 11.5 L 0.9 21.5 L 11.4 21.5"
        : num == 3 ? "M 1.8 0.5 L 10.05 0.5 L 5.55 8.5 L 7.8 8.5 L 9.3 9.5 L 10.05 10.5 L 10.8 13.5 L 10.8 15.5 L 10.05 18.5 L 8.55 20.5 L 6.3 21.5 L 4.05 21.5 L 1.8 20.5 L 1.05 19.5 L 0.3 17.5"
        : num == 4 ? "M 8.25 0.3 L 0.75 14.3 L 12 14.3 M 8.25 0.3 L 8.25 21.3"
        : num == 5 ? "M 9.3 0.5 L 1.8 0.5 L 1.05 9.5 L 1.8 8.5 L 4.05 7.5 L 6.3 7.5 L 8.55 8.5 L 10.05 10.5 L 10.8 13.5 L 10.8 15.5 L 10.05 18.5 L 8.55 20.5 L 6.3 21.5 L 4.05 21.5 L 1.8 20.5 L 1.05 19.5 L 0.3 17.5"
        : num == 6 ? "M 9.375 3.5 L 8.625 1.5 L 6.375 0.5 L 4.875 0.5 L 2.625 1.5 L 1.125 4.5 L 0.375 9.5 L 0.375 14.5 L 1.125 18.5 L 2.625 20.5 L 4.875 21.5 L 5.625 21.5 L 7.875 20.5 L 9.375 18.5 L 10.125 15.5 L 10.125 14.5 L 9.375 11.5 L 7.875 9.5 L 5.625 8.5 L 4.875 8.5 L 2.625 9.5 L 1.125 11.5 L 0.375 14.5"
        : num == 7 ? "M 10.5 0.5 L 3 21.5 M 0 0.5 L 10.5 0.5"
        : num == 8 ? "M 4.125 0.5 L 1.875 1.5 L 1.125 3.5 L 1.125 5.5 L 1.875 7.5 L 3.375 8.5 L 6.375 9.5 L 8.625 10.5 L 10.125 12.5 L 10.875 14.5 L 10.875 17.5 L 10.125 19.5 L 9.375 20.5 L 7.125 21.5 L 4.125 21.5 L 1.875 20.5 L 1.125 19.5 L 0.375 17.5 L 0.375 14.5 L 1.125 12.5 L 2.625 10.5 L 4.875 9.5 L 7.875 8.5 L 9.375 7.5 L 10.125 5.5 L 10.125 3.5 L 9.375 1.5 L 7.125 0.5 L 3.75 0.5"
        :            "M 10.125 7.5 L 9.375 10.5 L 7.875 12.5 L 5.625 13.5 L 4.875 13.5 L 2.625 12.5 L 1.125 10.5 L 0.375 7.5 L 0.375 6.5 L 1.125 3.5 L 2.625 1.5 L 4.875 0.5 L 5.625 0.5 L 7.875 1.5 L 9.375 3.5 L 10.125 7.5 L 10.125 12.5 L 9.375 17.5 L 7.875 20.5 L 5.625 21.5 L 4.125 21.5 L 1.875 20.5 L 1.125 18.5";

        size_t pos = 0;
        do {
            size_t n = p.find_first_of("ML", pos +1);
            if (n == std::string::npos)
                n = p.length() +1;

            std::string sub = p.substr(pos, n -1 -pos);
            spi el;
            el.is_move = sub[0] == 'M';
            size_t sep = sub.find(" ", 2);
            el.pt = QPointF(stod(sub.substr(2, sep -2)), stod(sub.substr(sep +1))) * ech;
            maxX = std::max(maxX, el.pt.x());

            el.pt += O;
            lspi.push_back(el);

            pos = n;
        } while (pos < p.length());
        O += QPointF(maxX+1, 0);
    }

    QPointF delta(O.x()/2, 28 * ech);
    for (auto&& p : lspi) {
        if (angle !=0)
            p.pt = rotatePt(p.pt, delta,  qDegreesToRadians(angle));
        p.pt += orig - delta;

        if (p.is_move)
            svgPathMoveTo(path, p.pt);
        else
            svgPathLineTo(path, p.pt);
    }
}
