#include "poigneerotation.h"
#include "unfold.h"

#include <QCursor>

PoigneeRotation::PoigneeRotation(const QRectF &rect, QGraphicsItem *parent, Unfold *d)
 : QGraphicsRectItem(rect, parent), donnees(d), en_cours(false), commence(false)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setCursor(QCursor(Qt::ClosedHandCursor));
}

QVariant PoigneeRotation::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        if (!commence) {
            ptDepart = pos();
            commence = true;
        }

        QLineF l = donnees->poigneeR->line();
        ptFin = value.toPointF();
        l.setP2(l.p2() - pos() + ptFin);
        donnees->poigneeR->setLine(l);
        return ptFin;
    }
    return QGraphicsItem::itemChange(change, value);
}

void PoigneeRotation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    en_cours = true;
    //commence = false;

    QGraphicsItem::mousePressEvent(event);
}

void PoigneeRotation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QLineF l = donnees->poigneeR->line();
    qreal a= l.angle();
    //qDebug() << "ANGLE :" << a;
    commence = false;
    en_cours = false;
    donnees->rotatePieceCourante(a);
    donnees->deja = false;
    QGraphicsItem::mouseReleaseEvent(event);
    donnees->displayUI();
}
