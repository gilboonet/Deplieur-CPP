#ifndef NUMITEM_H
#define NUMITEM_H

#include <QGraphicsSimpleTextItem>
#include <QString>

#include "titleitem.h"
#include "unfold.h"

class NumItem : public QGraphicsSimpleTextItem
{
public:
    TitleItem *ti;
    int nF;
    int fid;

    NumItem(QString, int, int);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
};

#endif // NUMITEM_H
