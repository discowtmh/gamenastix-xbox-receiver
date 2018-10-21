// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#pragma once

#include <fstream>
#include <memory>

#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPixmap>

class Compass {
public:
  Compass(QLabel &canvas, QLineEdit &status);

  void update(uint16_t positionDegrees, bool isActive = true);

  uint64_t lastReceivedPacket;

  uint16_t getPositionDegrees();
  void setPositionShift();

private:
  QLabel &canvas;
  QLineEdit &status;

  std::unique_ptr<QPainter> painter;
  std::unique_ptr<QPixmap> pixmap;

  uint16_t positionDegrees;
  uint16_t positionShift;
};
