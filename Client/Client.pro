QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      =  Client
TEMPLATE    =  app

SOURCES     += main.cpp\
               mainwindow.cpp \
               windowdragger.cpp \
               DarkStyle.cpp


HEADERS     += mainwindow.h \
               windowdragger.h \
               DarkStyle.h


FORMS       += mainwindow.ui

RESOURCES   += darkstyle.qrc
