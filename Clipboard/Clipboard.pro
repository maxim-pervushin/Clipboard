#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T14:13:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Clipboard
TEMPLATE = app


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    application.cpp \
    clipboardmanager.cpp

HEADERS  += \
    mainwindow.h \
    application.h \
    clipboardmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
