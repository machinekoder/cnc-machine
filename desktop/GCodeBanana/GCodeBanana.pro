#-------------------------------------------------
#
# Project created by QtCreator 2013-04-24T12:25:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCodeBanana
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qearleyparser.cpp \
    qgcodeparser.cpp \
    codeeditor.cpp

HEADERS  += mainwindow.h \
    qearleyparser.h \
    qgcodeparser.h \
    codeeditor.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
