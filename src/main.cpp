// This file is a part of Treadmill project.
// Copyright 2018 Disco WTMH S.A.

#include "SerialTesterMainWindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialTesterMainWindow w;
    w.show();

    return a.exec();
}
