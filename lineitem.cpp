#include "lineitem.h"

#include <QPen>

LineItem::LineItem(QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    m_outlineWidth(1),
    m_outlineColor(Qt::red),
    m_typeLigne(TL_NONE)
{
    setAcceptHoverEvents(true);
}

qreal LineItem::outlineWidth() const
{
    return m_outlineWidth;
}

void LineItem::setOutlineWidth(qreal outlineWidth)
{
    m_outlineWidth = outlineWidth;
}

QColor LineItem::outlineColor() const
{
    return m_outlineColor;
}

void LineItem::setOutlineColor(const QColor &outlineColor)
{
    m_outlineColor = outlineColor;
}

TypeLigne LineItem::typeLigne() const
{
    return m_typeLigne;
}

void LineItem::setTypeLigne(const TypeLigne t)
{
    m_typeLigne = t;
}

void LineItem::setCustomPen() {
    QPen p = QPen(m_outlineColor, m_outlineWidth);
    QVector<qreal> dashes;

    switch (m_typeLigne) {
        case TL_LIE_V: {
            dashes << 8 << 2 << 1 << 2;
            p.setStyle(Qt::CustomDashLine);
            p.setDashPattern(dashes);
            break;
        }
        case TL_LIE_M: {
            dashes << 8 << 8;
            p.setStyle(Qt::CustomDashLine);
            p.setDashPattern(dashes);
            break;
        }
        default: p.setStyle(Qt::SolidLine);
    }
    setPen(p);
}

void LineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    int a = data(1).toInt();
    int b = data(2).toInt();

    TitleItem * ti = static_cast<TitleItem*>(parentItem());
    if (!ti->donnees->flash)
        return;

    QPointF p1, p2;
    p1 = line().center() + parentItem()->pos();
    for (auto&& it : scene()->items()) {
        if ((it->data(1).toInt() == b) && (it->data(2).toInt() == a)) {
            QGraphicsLineItem* l2 = static_cast<QGraphicsLineItem*>(it);
            p2 = l2->line().center() + l2->parentItem()->pos();
            ti->donnees->flash->setLine(QLineF(p1, p2));
            ti->donnees->flash->setVisible(true);
            break;
        }
    }
    setPen(QPen(Qt::yellow, 2));
}

void LineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setCustomPen();
    TitleItem * ti = static_cast<TitleItem*>(parentItem());
    if (ti->donnees->flash)
        ti->donnees->flash->setVisible(false);
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    //qDebug() << "Custom item clicked.";
    TitleItem * ti = static_cast<TitleItem*>(parentItem());

    int type = data(0).toInt();
    int a = data(1).toInt();
    int b = data(2).toInt();

    qInfo("LINE ACTION : %d %d %d", type, a, b);

    type == -1 ? ti->donnees->splitPiece(a, b) : ti->donnees->stickPiece(a, b);
}
