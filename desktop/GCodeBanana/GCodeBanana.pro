#-------------------------------------------------
#
# Project created by QtCreator 2013-04-24T12:25:19
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GCodeBanana
TEMPLATE = app

LIBS += -lm -lusb


SOURCES += main.cpp\
        mainwindow.cpp \
    qearleyparser.cpp \
    qgcodeparser.cpp \
    codeeditor.cpp \
    communicator.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    qearleyparser.h \
    qgcodeparser.h \
    codeeditor.h \
    communicator.h \
    worker.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    settings/grammar.txt
