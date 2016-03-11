INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/backmodule.h

SOURCES += \
    $$PWD/backmodule.cpp

include(./semiot-gateway-udp-module/semiot-gateway-udp-module.pri)
