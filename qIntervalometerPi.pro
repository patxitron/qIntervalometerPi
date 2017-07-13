#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T17:00:24
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG   += c++11

TARGET    = qIntervalometerPi
TEMPLATE  = app

CONFIG(debug, debug|release):DEFINES+=DEBUG


SOURCES += main.cpp\
        mainwindow.cpp \
    touchspin.cpp \
    camera.cpp \
    intervalometer.cpp

HEADERS  += mainwindow.hpp \
    touchspin.hpp \
    camera.hpp \
    intervalometer.hpp

FORMS    +=

RESOURCES += \
    icons.qrc
