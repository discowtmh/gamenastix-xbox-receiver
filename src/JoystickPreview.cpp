// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#include "JoystickPreview.h"

#define JOYSTICK_CANVAS_DIM 100
#define JOYSTICK_CENTER (JOYSTICK_CANVAS_DIM / 2)
#define JOYSTICK_MARKER 5


JoystickPreview::JoystickPreview(QLabel& leftCanvas, QLabel& rightCanvas, QLabel& leftTriggerCanvas, QLabel& rightTriggerCanvas)
        : leftCanvas(leftCanvas)
        , rightCanvas(rightCanvas)
        , leftTriggerCanvas(leftTriggerCanvas)
        , rightTriggerCanvas(rightTriggerCanvas)
{
    pixmap = std::make_unique<QPixmap>(100, 100);
    pixmapTrigger = std::make_unique<QPixmap>(50, 100);
    painter = std::make_unique<QPainter>(pixmap.get());
    painterTrigger = std::make_unique<QPainter>(pixmapTrigger.get());
}

int scaleStick(float position)
{
    return static_cast<int>(JOYSTICK_CENTER + position * static_cast<float>(JOYSTICK_CENTER - 2 * JOYSTICK_MARKER));
}

int scaleTrigger(float position)
{
    return static_cast<int>(JOYSTICK_MARKER * 2 + position * static_cast<float>(2*JOYSTICK_CENTER - 4 * JOYSTICK_MARKER));
}

void JoystickPreview::update(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ)
{
    pixmap->fill();

    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPoint(scaleStick(leftX), scaleStick(leftY)), JOYSTICK_MARKER, JOYSTICK_MARKER);

    leftCanvas.setPixmap(*pixmap);

    pixmap->fill();

    painter->setBrush(Qt::darkCyan);
    painter->drawEllipse(QPoint(scaleStick(rightX), scaleStick(rightY)), JOYSTICK_MARKER, JOYSTICK_MARKER);
    rightCanvas.setPixmap(*pixmap);

    pixmapTrigger->fill();

    painterTrigger->setBrush(Qt::blue);
    painterTrigger->drawEllipse(QPoint(JOYSTICK_CENTER / 2, scaleTrigger(leftZ)), JOYSTICK_MARKER * 3, JOYSTICK_MARKER);

    leftTriggerCanvas.setPixmap(*pixmapTrigger);

    pixmapTrigger->fill();

    painterTrigger->setBrush(Qt::darkCyan);
    painterTrigger->drawEllipse(QPoint(JOYSTICK_CENTER / 2, scaleTrigger(rightZ)), JOYSTICK_MARKER * 3, JOYSTICK_MARKER);

    rightTriggerCanvas.setPixmap(*pixmapTrigger);
}
