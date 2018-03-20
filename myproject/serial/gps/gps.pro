#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T14:26:13
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gps
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serials.cpp

HEADERS  += mainwindow.h \
    serials.h

FORMS    += mainwindow.ui
