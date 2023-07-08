#ifndef NUMITEM_H
#define NUMITEM_H

#include <QGraphicsSimpleTextItem>
#include <QString>

//#include "titleitem.h"
#include "pieceitem.h"
#include "unfold.h"

class NumItem : public QGraphicsSimpleTextItem
{
public:
    PieceItem *ti;
    int nF;
    int fid;

    NumItem(QString, int, int);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
};

#endif // NUMITEM_H
