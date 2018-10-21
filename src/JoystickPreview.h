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
    JoystickPreview(QLabel& leftCanvas, QLabel& rightCanvas);
    void update(float leftX, float leftY, float rightX, float rightY);

private:
    QLabel& leftCanvas;
    QLabel& rightCanvas;
    std::unique_ptr<QPainter> painter;
    std::unique_ptr<QPixmap> pixmap;
};
