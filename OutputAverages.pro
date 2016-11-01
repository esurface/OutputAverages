#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T10:47:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OutputAverages
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        stats.cpp

HEADERS  += mainwindow.h \
        stats.h

FORMS    += mainwindow.ui

DISTFILES += \
    docs/sample.out
