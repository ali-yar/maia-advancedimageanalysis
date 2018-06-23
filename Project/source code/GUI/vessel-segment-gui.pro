#-------------------------------------------------
#
# Project created by QtCreator 2018-05-14T19:55:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vessel-segment-gui
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

CONFIG += 64bit

QMAKE_LFLAGS += /STACK:900000000

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        vesseldetector.cpp \
        utils.cpp

HEADERS += \
        mainwindow.h \
        vesseldetector.h \
        utils.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:\Programs\opencv\opencv2\build\include

#LIBS += -LC:/Programs/opencv/opencv2/build/x64/vc14/lib \
#        -lopencv_core2413d \
#        -lopencv_highgui2413d \
#        -lopencv_imgproc2413d \
#        -lopencv_photo2413 \
#        -lopencv_features2d2413d \
#        -lopencv_calib3d2413

LIBS += -LC:/Programs/opencv/opencv2/build/x64/vc14/lib \
        -lopencv_core2413 \
        -lopencv_highgui2413 \
        -lopencv_imgproc2413 \
        -lopencv_photo2413
