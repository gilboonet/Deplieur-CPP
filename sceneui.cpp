#include "sceneui.h"

//#include <QDebug>

SceneUI::SceneUI(QObject *parent)
    : QGraphicsScene{parent}
{
    connect(this, &QGraphicsScene::selectionChanged, this, &SceneUI::sceneSelChange);
    //BandeSelection = nullptr;
}
/*
void SceneUI::mousePressEvent(QMouseEvent *event)
{
    BSorigin = event->pos();
    if (!BandeSelection)
        BandeSelection = new QRubberBand(QRubberBand::Rectangle);
    BandeSelection->setGeometry(QRect(BSorigin, QSize()));
    BandeSelection->show();
}

void SceneUI::mouseMoveEvent(QMouseEvent *event)
{
    BandeSelection->setGeometry(QRect(BSorigin, event->pos()).normalized());
}

void SceneUI::mouseReleaseEvent(QMouseEvent *event)
{
    BandeSelection->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}
*/
void SceneUI::sceneSelChange() {
    //qInfo() << "selection changee";
}

