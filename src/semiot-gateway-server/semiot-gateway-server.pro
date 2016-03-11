QMAKE_CXXFLAGS += -std=c++11
QT += core websockets network qml
QT -= gui

TARGET = semiot-gateway-server
CONFIG += console network

TEMPLATE = app

SOURCES += main.cpp \
    devicesconfigsloader.cpp \
    deviceconfig.cpp \
    dataserver.cpp

HEADERS += \
    devicesconfigsloader.h \
    deviceconfig.h \
    dataserver.h

RESOURCES += \
    res.qrc

include(./modules/modules.pri) #TODO: shared lib

DISTFILES += \
    api.doc.json \
    api.json \
    ../../doc/semiot-gateway.qmodel

LIBS += -lrdf
LIBS += -ldataquay
