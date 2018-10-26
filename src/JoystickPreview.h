// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#pragma once

#include <memory>

#include <QLabel>
#include <QPainter>
#include <QPixmap>

class JoystickPreview
{
public:
    JoystickPreview(QLabel& leftCanvas, QLabel& rightCanvas, QLabel& leftTriggerCanvas, QLabel& rightTriggerCanvas);
    void update(float leftX, float leftY, float leftZ, float rightX, float rightY, float rightZ);

private:
    QLabel& leftCanvas;
    QLabel& rightCanvas;
    QLabel& leftTriggerCanvas;
    QLabel& rightTriggerCanvas;
    std::unique_ptr<QPainter> painter;
    std::unique_ptr<QPainter> painterTrigger;
    std::unique_ptr<QPixmap> pixmap;
    std::unique_ptr<QPixmap> pixmapTrigger;
};
