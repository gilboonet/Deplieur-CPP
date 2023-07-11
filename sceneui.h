#ifndef SCENEUI_H
#define SCENEUI_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QRubberBand>

class SceneUI : public QGraphicsScene
{
public:
    explicit SceneUI(QObject *parent = nullptr);
//    QRubberBand *BandeSelection;
//    QPoint BSorigin;

protected:
    void sceneSelChange();
//    void mousePressEvent(QMouseEvent *);
//    void mouseMoveEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif // SCENEUI_H
