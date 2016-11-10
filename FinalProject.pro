#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T12:07:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FinalProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    world.h \
    world_global.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libworld.so/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libworld.so/debug/ -lworld
else:unix: LIBS += -L$$PWD/libworld.so/ -lworld

INCLUDEPATH += $$PWD/libworld.so
DEPENDPATH += $$PWD/libworld.so

RESOURCES += \
    resource.qrc
