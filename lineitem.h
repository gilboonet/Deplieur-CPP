#ifndef LINEITEM_H
#define LINEITEM_H

//#include "titleitem.h"
#include "pieceitem.h"
#include "unfold.h"

#include <QGraphicsLineItem>

enum TypeLigne {
    TL_NONE,
    TL_LIE_M,
    TL_LIE_C,
    TL_LIE_V,
    TL_COUPE_M,
    TL_COUPE_C,
    TL_COUPE_V,
    TL_LANG_M,
    TL_LANG_C,
    TL_LANG_V
};

class LineItem : public QGraphicsLineItem
{//Q_OBJECT
public:
    explicit LineItem(QGraphicsItem *parent = nullptr);

    qreal outlineWidth() const;
    void setOutlineWidth(qreal outlineWidth);

    QColor outlineColor() const;
    void setOutlineColor(const QColor &outlineColor);

    TypeLigne typeLigne() const;
    void setTypeLigne(TypeLigne t);

    void setCustomPen();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void sendMsg(le_data);

private:
    qreal m_outlineWidth;
    QColor m_outlineColor;
    TypeLigne m_typeLigne;
};

#endif // LINEITEM_H
