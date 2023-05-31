#ifndef TITLEITEM_H
#define TITLEITEM_H

#include <QGraphicsSimpleTextItem>
#include <QString>

class Unfold;

class TitleItem : public QGraphicsSimpleTextItem
{
public:
    Unfold *donnees;
    TitleItem(const QString, Unfold *);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    //QVariant itemChange(GraphicsItemChange, const QVariant &);
};

#endif // TITLEITEM_H
