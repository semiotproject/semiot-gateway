QMAKE_CXXFLAGS += -std=c++11
QT += core websockets network qml
QT -= gui

TARGET = semiot-gateway-server
CONFIG += console network
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketserver.cpp \
    udpdriver.cpp \
    devicesconfigsloader.cpp \
    deviceconfig.cpp \
    httpserver.cpp \
    httprequestcontroller.cpp

HEADERS += \
    websocketserver.h \
    udpdriver.h \
    devicesconfigsloader.h \
    deviceconfig.h \
    httpserver.h \
    httprequestcontroller.h

RESOURCES += \
    res.qrc

# http://stefanfrings.de/qtwebapp/index-en.html:
include(./3rdparty/QtWebApp/httpserver/httpserver.pri) #TODO: shared lib

DISTFILES += \
    api.doc.json \
    api.json
