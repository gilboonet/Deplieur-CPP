#ifndef VUEUI_H
#define VUEUI_H

#include <QGraphicsView>
#include <QObject>
//#include <QRubberBand>

class vueUI : public QGraphicsView
{
public:
    vueUI();

protected:
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

private:
    //QRubberBand *bandeSelection = nullptr;
    //QPoint  origine;
};

#endif // VUEUI_H
