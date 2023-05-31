#include "rotationslider.h"

RotationSlider::RotationSlider() :
    QSlider(Qt::Horizontal)
{
    setRange(-180,180);
    setValue(0);
    setStyleSheet("height:8px");
    //setVisible(false);
}
