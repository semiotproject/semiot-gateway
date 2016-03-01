#ifndef HTTPREQUESTCONTROLLER_H
#define HTTPREQUESTCONTROLLER_H

#include <QObject>
#include <QUrl>
#include "httprequesthandler.h"
#include "dataserver.h"

class HttpRequestController : public HttpRequestHandler
{
    Q_OBJECT
signals:
    void addDeviceDriverFromUrl(QUrl url);
    void addDeviceDriverFromString(QString string);
    void newRequestReceived(QVariant params);
public:
    explicit HttpRequestController(DataServer& ds, QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    const DataServer& _dataServer;
};

#endif // HTTPREQUESTCONTROLLER_H
