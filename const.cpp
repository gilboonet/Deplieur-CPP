#include <cmath>
#include <numbers>

#include "const.h"

int prev(const int n) { // returns previous value from triplet(0,1,2)
    return n > 0 ? n - 1 : 2;
}

int next(const int n) {  // returns next value from triplet(0,1,2)
    return n < 2 ? n + 1 : 0;
}

bool eq(float a, float b) {
    return std::abs(a - b) < epsilon;
}

bool eq(QVector2D a, QVector2D b) {
    return eq(a.x(), b.x()) && eq(a.y(), b.y());
}

float radToDeg(float r) {
    return (r * 180) / std::numbers::pi;
}

float degToRad(float d) {
    return d * std::numbers::pi / 180;
}

float calc_angle(QVector2D a, QVector2D b, QVector2D c) {
    QVector2D ab = b - a;
    QVector2D ac = c - a;

    float rot_ab_ac = atan2(ac.y() * ab.x() - ac.x() * ab.y(), ac.x() * ab.x() + ac.y() * ab.y());
    return rot_ab_ac;
}

int sgn(float x) {
   if (x < 0) return 1;
   if (x > 0) return -1;
   return 0;
}

float cmpo(float x) { return x * 2.54;}

QVector2D cmpo(QVector2D v) { return v * 2.54;}

Triangle2d cmpo(Triangle2d t) { return t * 2.54;}

QVector2D middle (QVector2D a, QVector2D b) {
    return (a + b) / 2;
}

float distance(const QVector2D& v1, const QVector2D& v2) {
    QVector2D d = v2 - v1;
    return sqrt((d.x() * d.x()) +(d.y() * d.y()));
}

float angle(const QVector2D& v, const QVector2D& p) {
    return atan2(p.y() - v.y(), p.x() - v.x());
}

QVector2D rotatePt(const QVector2D& v, const QVector2D& c, const float a) {
    float lcos = cos(a);
    float lsin = sin(a);

    QVector2D r = QVector2D(
       (lcos *(v.x() - c.x())) +(lsin *(v.y() - c.y())) + c.x(),
       (lcos *(v.y() - c.y())) -(lsin *(v.x() - c.x())) + c.y()
    );
    return r;
}

float direction(const QVector2D& p1, const QVector2D& p2) {
    return atan2(p2.y() - p1.y(), p2.x() - p1.x());
}

void svgPathMoveTo(SVG::Path* path, QPointF point) {
    path->move_to(point.x(), point.y());
}

void svgPathLineTo(SVG::Path* path, QPointF point) {
    path->line_to(point.x(), point.y());
}

QPointF rotatePt(const QPointF& v, const QPointF& c, const float a) {
    float lcos = cos(a);
    float lsin = sin(a);

    QPointF r = QPointF(
       (lcos *(v.x() - c.x())) +(lsin *(v.y() - c.y())) + c.x(),
       (lcos *(v.y() - c.y())) -(lsin *(v.x() - c.x())) + c.y()
    );
    return r;
}

void drawHersheyInt(SVG::Path *path, QPointF orig, int val, double angle, double ech = 1) {

struct spi {
    bool is_move;
    QPointF pt;
};

    std::vector<spi> lspi;
    QString s = QString::number(val);
    QPointF O{};
    double maxX = 0;
    for (const auto c : s) {
        int num = c.digitValue();
        std::string p =
          num == 0 ? "M 6.5 0.5 L 3.5 1.5 L 1.5 4.5 L 0.5 9.5 L 0.5 12.5 L 1.5 17.5 L 3.5 20.5 L 6.5 21.5 L 8.5 21.5 L 11.5 20.5 L 13.5 17.5 L 14.5 12.5 L 14.5 9.5 L 13.5 4.5 L 11.5 1.5 L 8.5 0.5 L 6.5 0.5"
        : num == 1 ? "M 0.223607 5.20711 L 2.22361 4.20711 L 5.22361 1.20711 L 5.22361 22.2071"
        : num == 2 ? "M 2.20711 5.5 L 2.20711 4.5 L 3.20711 2.5 L 4.20711 1.5 L 6.20711 0.5 L 10.2071 0.5 L 12.2071 1.5 L 13.2071 2.5 L 14.2071 4.5 L 14.2071 6.5 L 13.2071 8.5 L 11.2071 11.5 L 1.20711 21.5 L 15.2071 21.5"
        : num == 3 ? "M 2.44721 0.5 L 13.4472 0.5 L 7.44721 8.5 L 10.4472 8.5 L 12.4472 9.5 L 13.4472 10.5 L 14.4477 13.5 L 14.4477 15.5 L 13.4472 18.5 L 11.4472 20.5 L 8.44721 21.5 L 5.44721 21.5 L 2.44721 20.5 L 1.44721 19.5 L 0.447214 17.5"
        : num == 4 ? "M 10.9716 0.290619 L 0.971595 14.2906 L 15.9716 14.2906 M 10.9716 0.290619 L 10.9716 21.2906"
        : num == 5 ? "M 12.4472 0.5 L 2.44721 0.5 L 1.44721 9.5 L 2.44721 8.5 L 5.44721 7.5 L 8.44721 7.5 L 11.4472 8.5 L 13.4472 10.5 L 14.4472 13.5 L 14.4472 15.5 L 13.4472 18.5 L 11.4472 20.5 L 8.44721 21.5 L 5.44721 21.5 L 2.44721 20.5 L 1.44721 19.5 L 0.447214 17.5"
        //: num == 6 ? "M 15.3954 4.35526 L 14.3954 2.35526 L 11.3954 1.35526 L 9.39541 1.35526 L 6.39541 2.35526 L 4.39541 5.35526 L 3.39541 10.3553 L 3.39541 15.3553 L 4.39541 19.3553 L 6.39541 21.3553 L 9.39541 22.3553 L 10.3954 22.3553 L 13.3954 21.3553 L 15.3954 19.3553 L 16.3954 16.3553 L 16.3954 15.3553 L 15.3954 12.3553 L 13.3954 10.3553 L 10.3954 9.35526 L 9.39541 9.35526 L 6.39541 10.3553 L 4.39541 12.3553 L 3.39541 15.3553"
        : num == 6 ? "M 12.5 3.5 L 11.5 1.5 L 8.5 0.5 L 6.5 0.5 L 3.5 1.5 L 1.5 4.5 L 0.5 9.5 L 0.5 14.5 L 1.5 18.5 L 3.5 20.5 L 6.5 21.5 L 7.5 21.5 L 10.5 20.5 L 12.5 18.5 L 13.5 15.5 L 13.5 14.5 L 12.5 11.5 L 10.5 9.5 L 7.5 8.5 L 6.5 8.5 L 3.5 9.5 L 1.5 11.5 L 0.5 14.5"
        : num == 7 ? "M 14 0.5 L 4 21.5 M 0 0.5 L 14 0.5"
        : num == 8 ? "M 5.5 0.5 L 2.5 1.5 L 1.5 3.5 L 1.5 5.5 L 2.5 7.5 L 4.5 8.5 L 8.5 9.5 L 11.5 10.5 L 13.5 12.5 L 14.5 14.5 L 14.5 17.5 L 13.5 19.5 L 12.5 20.5 L 9.5 21.5 L 5.5 21.5 L 2.5 20.5 L 1.5 19.5 L 0.5 17.5 L 0.5 14.5 L 1.5 12.5 L 3.5 10.5 L 6.5 9.5 L 10.5 8.5 L 12.5 7.5 L 13.5 5.5 L 13.5 3.5 L 12.5 1.5 L 9.5 0.5 L 5.5 0.5"
        :            "M 13.5 7.5 L 12.5 10.5 L 10.5 12.5 L 7.5 13.5 L 6.5 13.5 L 3.5 12.5 L 1.5 10.5 L 0.5 7.5 L 0.5 6.5 L 1.5 3.5 L 3.5 1.5 L 6.5 0.5 L 7.5 0.5 L 10.5 1.5 L 12.5 3.5 L 13.5 7.5 L 13.5 12.5 L 12.5 17.5 L 10.5 20.5 L 7.5 21.5 L 5.5 21.5 L 2.5 20.5 L 1.5 18.5";

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

    QPointF delta(O.x()/2, 24 * ech);
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
