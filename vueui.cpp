#include "vueui.h"

//#include <QMouseEvent>
//#include <QRect>

vueUI::vueUI()
{
}

/*void vueUI::mousePressEvent(QMouseEvent *event)
{
    origine = event->pos();

    if (!bandeSelection) {
        bandeSelection = new QRubberBand(QRubberBand::Rectangle, this);
    }

    bandeSelection->setGeometry(QRect(origine, QSize()));
    bandeSelection->show();
}

void vueUI::mouseReleaseEvent(QMouseEvent *event)
{
    bandeSelection->hide();
}

void vueUI::mouseMoveEvent(QMouseEvent *event)
{
    bandeSelection->setGeometry(QRect(origine, event->pos()).normalized());
}*/
