#include "numitem.h"

#include <QCursor>
#include <QFont>
#include <QFontDatabase>

NumItem::NumItem(const QString text, int id1, int id2)
: QGraphicsSimpleTextItem(text), nF(id1), fid(id2)
{
    setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(QFont(fixedFont.family(), 9));
    setBrush(QBrush(Qt::black));
}

void NumItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    PieceItem* ti = static_cast<PieceItem*>(parentItem());
    ti->donnees->changeLanguette(nF, fid);
}

void NumItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(QBrush(Qt::yellow));
}

void NumItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setBrush(QBrush(Qt::black));

}
