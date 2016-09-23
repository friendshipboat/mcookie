#-------------------------------------------------
#
# Project created by QtCreator 2016-09-17T22:54:57
#
#-------------------------------------------------

QT       += core gui
LIBS += -lpthread libwsock32 libws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shipcontrol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newthread.cpp

HEADERS  += mainwindow.h \
    newthread.h

FORMS    += mainwindow.ui

RESOURCES += \
    shipresource.qrc
