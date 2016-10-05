QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LiveStream
TEMPLATE = app


SOURCES  += main.cpp\
            mainwindow.cpp \
            videothread.cpp

HEADERS  += mainwindow.h \
            videothread.h

FORMS    += mainwindow.ui

LIBS     += -lueye_api
