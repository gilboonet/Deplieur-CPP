#ifndef PIECEITEM_H
#define PIECEITEM_H

#include "piece.h"

#include <QGraphicsPolygonItem>

class Unfold;

class PieceItem : public QGraphicsPolygonItem
{//Q_OBJECT
public:
    PieceItem(const QPolygonF &polygon, Unfold *, Piece *);
    Unfold *donnees;
    Piece *piece;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // PIECEITEM_H
