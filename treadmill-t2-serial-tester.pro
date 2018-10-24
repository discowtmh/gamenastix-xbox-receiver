#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T08:41:09
#
#-------------------------------------------------

QT       += core gui widgets serialport

TARGET = treadmill-t2-serial-tester
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/
INCLUDEPATH += deps/treadmill-t2-driver-gamenastix-helper/include/
INCLUDEPATH += deps/treadmill-t2-biomodel/include/
INCLUDEPATH += deps/glm/

CONFIG += c++14

QMAKE_CXXFLAGS += -DGLM_ENABLE_EXPERIMENTAL

SOURCES +=                                                        \
    deps/treadmill-t2-biomodel/include/Frame.cpp                  \
    deps/treadmill-t2-biomodel/include/Mode.cpp                   \
    deps/treadmill-t2-biomodel/include/Parser.cpp                 \
    deps/treadmill-t2-biomodel/include/Part.cpp                   \
    deps/treadmill-t2-biomodel/include/Type.cpp                   \
    src/Compass.cpp                                               \
    src/JoystickPreview.cpp                                       \
    src/SerialTesterMainWindow.cpp                                \
    src/main.cpp                                                  \



HEADERS +=                                                        \
    deps/treadmill-t2-biomodel/include/Frame.h                    \
    deps/treadmill-t2-biomodel/include/Mode.h                     \
    deps/treadmill-t2-biomodel/include/Node.h                     \
    deps/treadmill-t2-biomodel/include/Parser.h                   \
    deps/treadmill-t2-biomodel/include/Part.h                     \
    deps/treadmill-t2-biomodel/include/Type.h                     \
    deps/treadmill-t2-biomodel/include/deepModel/Bio4.h           \
    deps/treadmill-t2-biomodel/include/deepModel/Medium.h         \
    deps/treadmill-t2-biomodel/include/deepModel/Model.h          \
    deps/treadmill-t2-biomodel/include/deepModel/Treadmill.h      \
    deps/treadmill-t2-driver-gamenastix-helper/include/Protocol.h \
    src/Compass.h                                                 \
    src/JoystickPreview.h                                         \
    src/SerialTesterMainWindow.h                                  \
    src/SystemClock.h                                             \


FORMS += \
    src/SerialTesterMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
