#ifndef POIGNEEROTATION_H
#define POIGNEEROTATION_H

#include <QGraphicsRectItem>
#include <QObject>

class Unfold;

class PoigneeRotation : public QGraphicsRectItem
{
public:
    PoigneeRotation(const QRectF &, QGraphicsItem *, Unfold *);
    Unfold *donnees;
    QPointF ptDepart, ptFin;
    bool en_cours, commence;

protected:
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // POIGNEEROTATION_H
