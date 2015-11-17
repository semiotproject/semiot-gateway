QMAKE_CXXFLAGS += -std=c++11
QT += core websockets network qml
QT -= gui

TARGET = semiot-gateway-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketserver.cpp \
    udpdriver.cpp \
    devicesconfigsloader.cpp

HEADERS += \
    websocketserver.h \
    udpdriver.h \
    devicesconfigsloader.h

RESOURCES += \
    res.qrc
