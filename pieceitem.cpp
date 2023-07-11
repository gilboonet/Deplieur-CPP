#include "pieceitem.h"

#include "unfold.h"

PieceItem::PieceItem(const QPolygonF &polygon, Unfold *d, Piece *p) :
    QGraphicsPolygonItem(polygon), donnees(d), piece(p)
{
    setCursor(QCursor(Qt::OpenHandCursor));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsGeometryChanges);
    setPen(Qt::NoPen);
    setZValue(0);
}

void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    donnees->setPieceCourante(this);
}

QVariant PieceItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        piece->O = piece->O - pos() + value.toPointF();
    }
    return QGraphicsItem::itemChange(change, value);
}

