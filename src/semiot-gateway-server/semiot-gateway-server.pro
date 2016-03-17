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
include(./utils/utils.pri) #TODO: shared lib
#include(./digitalbazaarjsonld/libjsonld.pri)

DISTFILES += \
    api.doc.json \
    api.json \
    ../../doc/semiot-gateway.qmodel

LIBS += -lrdf

LIBS += -ldataquay

# TODO: move to pkg-config or something:
LIBS += -lraptor2
INCLUDEPATH += /usr/include/raptor2
DEPENDPATH += /usr/include/raptor2

LIBS += -lrasqal
INCLUDEPATH += /usr/include/rasqal
DEPENDPATH += /usr/include/rasqal
