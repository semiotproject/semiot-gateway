INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/frontmodule.h \
    $$PWD/frontmodulemanager.h
SOURCES += $$PWD/frontmodule.cpp \
    $$PWD/frontmodulemanager.cpp

include(./semiot-gateway-http-module/semiot-gateway-http-module.pri)
include(./semiot-gateway-ws-module/semiot-gateway-ws-module.pri)
