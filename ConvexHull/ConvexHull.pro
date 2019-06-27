#-------------------------------------------------
#
# Project created by QtCreator 2019-06-04T22:18:09
#
#-------------------------------------------------

QT += core gui
QT += opengl
LIBS += -lglu32 -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConvexHull
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

CONFIG += c++17

LIBS += "C:\Users\Timoniche\Desktop\Graphics\build-ConvexHull-Desktop_Qt_5_12_0_MinGW_64_bit-Debug\debug\HullLib.dll"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    glworker.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    glworker.h \
    hulllib.h \
    hulllib_global.h

FORMS += \
        mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-HullLib-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/release/ -lHullLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-HullLib-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/debug/ -lHullLib
else:unix: LIBS += -L$$PWD/../build-HullLib-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/ -lHullLib

INCLUDEPATH += $$PWD/../build-HullLib-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/../build-HullLib-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/debug
