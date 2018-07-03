QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = stripchart
TEMPLATE = app

INCLUDEPATH += ../qcustomplot/
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        ../qcustomplot/qcustomplot.cpp \
        axistag.cpp

HEADERS += \
        mainwindow.h \
        ../qcustomplot/qcustomplot.h \
        axistag.h

FORMS += \
        mainwindow.ui
