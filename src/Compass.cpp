// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#include "Compass.h"

#include <cmath>

#include <qmath.h>

#include "SystemClock.h"


Compass::Compass(QLabel& canvas, QLineEdit& status)
        : canvas(canvas)
        , status(status)
{
    pixmap = std::make_unique<QPixmap>(100, 100);
    painter = std::make_unique<QPainter>(pixmap.get());

    this->positionDegrees = 0;
    this->positionShift = 0;

    lastReceivedPacket = SystemClock::millis();
}

#define COMPASS_X_CENTER 50
#define COMPASS_Y_CENTER 50
#define COMPASS_RADIUS 40
#define COMPASS_MARKER 5

void Compass::setPositionShift()
{
    this->positionShift = this->positionDegrees;
    update(this->positionDegrees);
}


uint16_t Compass::getPositionDegrees()
{
    return (360 + this->positionDegrees - this->positionShift) % 360;
}

void Compass::update(uint16_t positionDegrees, bool isActive)
{
    this->lastReceivedPacket = SystemClock::millis();

    this->positionDegrees = positionDegrees;

    pixmap->fill();

    painter->setBrush(Qt::gray);

    painter->drawEllipse(QPoint(COMPASS_X_CENTER, COMPASS_Y_CENTER), COMPASS_RADIUS, COMPASS_RADIUS);

    int xPosition = (COMPASS_X_CENTER)
                    + static_cast<int>(static_cast<double>(COMPASS_RADIUS)
                                       * cos(M_PI * static_cast<double>(getPositionDegrees()) / 180.0));
    int yPosition = (COMPASS_X_CENTER)
                    - static_cast<int>(static_cast<double>(COMPASS_RADIUS)
                                       * sin(M_PI * static_cast<double>(getPositionDegrees()) / 180.0));

    if (isActive)
    {
        painter->setBrush(Qt::green);
    }
    else
    {
        painter->setBrush(Qt::red);
    }
    painter->drawEllipse(QPoint(xPosition, yPosition), COMPASS_MARKER, COMPASS_MARKER);

    canvas.setPixmap(*pixmap);

    status.setText(QString::number(getPositionDegrees()));
}
