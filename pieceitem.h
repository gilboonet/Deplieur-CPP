#ifndef PIECEITEM_H
#define PIECEITEM_H

#include <QGraphicsPolygonItem>

class Unfold;

class PieceItem : public QGraphicsPolygonItem
{
public:
    int num;
    Unfold *donnees;
    PieceItem(const QPolygonF &polygon, int, Unfold *);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

};

#endif // PIECEITEM_H
