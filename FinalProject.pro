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
        mainwindow.cpp \
    node.cpp \
    game.cpp

HEADERS  += mainwindow.h \
    lib\world.h \
    lib\world_global.h \
    lib/world.h \
    lib/world_global.h \
    node.h \
    game.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libworld.so/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libworld.so/debug/ -lworld
else:unix: LIBS += -L$$PWD/libworld.so/ -lworld

INCLUDEPATH += $$PWD/libworld.so
DEPENDPATH += $$PWD/libworld.so

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lworld
else:unix: LIBS += -L$$PWD/lib/ -lworld

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lworld
else:unix: LIBS += -L$$PWD/lib/ -lworld

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
