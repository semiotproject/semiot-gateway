#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "frontmodule.h"

//#include <QObject>
//#include <QUrl>
#include "httplistener.h"
#include "httprequestcontroller.h"
//#include "dataserver.h"

class HttpServer : public FrontModule
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);

signals:
    // void addDeviceDriverFromUrl(QUrl url);
    // void addDeviceDriverFromString(QString string);
    // void newRequestReceived(QVariant params);

private:
    HttpListener* _listener;
    QSettings* _settings;
    HttpRequestController* _requestController;
};

#endif // HTTPSERVER_H
