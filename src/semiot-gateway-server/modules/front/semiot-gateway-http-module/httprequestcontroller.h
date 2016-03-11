#ifndef HTTPREQUESTCONTROLLER_H
#define HTTPREQUESTCONTROLLER_H

#include <QObject>
#include <QUrl>
#include "httprequesthandler.h"
#include "dataserver.h" // Strange architechture due to qtwebapp

class HttpRequestController : public HttpRequestHandler
{
    Q_OBJECT
signals:
    void addDeviceDriverFromUrl(QUrl url);
    void addDeviceDriverFromString(QString string);
    void newRequestReceived(QVariant params);
public:
    explicit HttpRequestController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
    void setDataServer(DataServer* ds);
private:
    DataServer* _dataServer;

};

#endif // HTTPREQUESTCONTROLLER_H
