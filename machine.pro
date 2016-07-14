#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T09:02:21
#
#-------------------------------------------------

QT       += core gui
#...
LIBS += -lpthread libwsock32 libws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = machine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
