// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#include "JoystickPreview.h"


JoystickPreview::JoystickPreview(QLabel& leftCanvas, QLabel& rightCanvas)
        : leftCanvas(leftCanvas)
        , rightCanvas(rightCanvas)
{
    pixmap = std::make_unique<QPixmap>(100, 100);
    painter = std::make_unique<QPainter>(pixmap.get());
}

#define JOYSTICK_CENTER 50
#define JOYSTICK_MARKER 5

int scale(float position)
{
    return static_cast<int>(JOYSTICK_CENTER + position * static_cast<float>(JOYSTICK_CENTER - 2 * JOYSTICK_MARKER));
}

void JoystickPreview::update(float leftX, float leftY, float rightX, float rightY)
{
    pixmap->fill();

    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPoint(scale(leftX), scale(leftY)), JOYSTICK_MARKER, JOYSTICK_MARKER);

    leftCanvas.setPixmap(*pixmap);

    pixmap->fill();

    painter->setBrush(Qt::darkCyan);
    painter->drawEllipse(QPoint(scale(rightX), scale(rightY)), JOYSTICK_MARKER, JOYSTICK_MARKER);
    rightCanvas.setPixmap(*pixmap);
}
