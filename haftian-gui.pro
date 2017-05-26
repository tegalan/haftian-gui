#-------------------------------------------------
#
# Project created by QtCreator 2017-05-17T13:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = haftian-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    listusbdrive.cpp

HEADERS  += mainwindow.h \
    listusbdrive.h

FORMS    += mainwindow.ui

LIBS += -ludev
