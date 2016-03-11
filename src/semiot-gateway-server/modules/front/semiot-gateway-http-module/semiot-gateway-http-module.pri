INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# http://stefanfrings.de/qtwebapp/index-en.html:
include(./3rdparty/QtWebApp/httpserver/httpserver.pri) #TODO: shared lib

HEADERS += $$PWD/httpserver.h \
    $$PWD/httprequestcontroller.h
SOURCES += $$PWD/httpserver.cpp \
    $$PWD/httprequestcontroller.cpp
