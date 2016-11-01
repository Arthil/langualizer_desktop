#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T15:58:46
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = langualizer_no_graphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addchapter.cpp \
    addbooks.cpp \
    addwords.cpp \
    relaxmode.cpp \
    questionsettings.cpp

HEADERS  += mainwindow.h \
    addchapter.h \
    addbooks.h \
    addwords.h \
    relaxmode.h \
    questionsettings.h

FORMS    += mainwindow.ui \
    addchapter.ui \
    addbooks.ui \
    addwords.ui \
    relaxmode.ui
