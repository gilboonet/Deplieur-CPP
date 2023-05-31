#ifndef ROTATIONSLIDER_H
#define ROTATIONSLIDER_H

#include <QSlider>

#include "piece.h"

class RotationSlider : public QSlider
{
public:
    Piece* piece;
    RotationSlider();
};

#endif // ROTATIONSLIDER_H
