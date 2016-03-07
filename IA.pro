#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T01:34:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lfuzzylite-static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lfuzzylite-static
else:unix: LIBS += -L$$PWD/../ -lfuzzylite-static

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libfuzzylite-static.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libfuzzylite-static.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/fuzzylite-static.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/fuzzylite-static.lib
else:unix: PRE_TARGETDEPS += $$PWD/../libfuzzylite-static.a
