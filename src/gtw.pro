QMAKE_CXXFLAGS += -std=c++11
QT += core websockets network qml
QT -= gui

TARGET = gtw
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketserver.cpp \
    udpdriver.cpp

HEADERS += \
    websocketserver.h \
    udpdriver.h

