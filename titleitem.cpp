#include "titleitem.h"

//#include <QFocusEvent>
#include <QFont>
#include <QObject>
#include <QPen>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

#include "page.h"
#include "piece.h"
#include "unfold.h"

TitleItem::TitleItem(const QString text, Unfold *d) :
    QGraphicsSimpleTextItem(text), donnees(d)
{
    setFont(QFont("Courier", 15));
    setPen(QPen(Qt::blue, 1));
    setAcceptHoverEvents(true);
    setCursor(QCursor(Qt::OpenHandCursor));
    setFlags(QGraphicsItem::ItemIsFocusable
    //| QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemIsMovable);

}

void TitleItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int n = text().toInt()-1;
    donnees->setPieceCourante(n, this);
    Piece *pieceCourante = donnees->getPieceCourante();

    Page* pageCourante = nullptr;
    for (auto&& pg : donnees->pages) {
        for (auto&& p : pg.pieces) {
            if (p == *pieceCourante) {
                pageCourante = &pg;
                break;
            }
        }
        if (pageCourante)
            break;
    }

    if (pageCourante && pieceCourante) {
        donnees->rSlider->setEnabled(true);
        donnees->rSlider->setValue(pieceCourante->a);
    }
}

/*QVariant TitleItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        Piece *pieceCourante = donnees->getPieceCourante();
        if (pieceCourante) {
            //qInfo() << QVector2D(newPos);
            pieceCourante->O = QVector2D(
            pieceCourante->O.x() + this->pos().x() - newPos.x(),
            pieceCourante->O.y() + this->pos().y() - newPos.y());
        }

    }
    return QGraphicsItem::itemChange(change, value);
}*/
