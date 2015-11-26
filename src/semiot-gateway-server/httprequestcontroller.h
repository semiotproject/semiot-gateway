#ifndef HTTPREQUESTCONTROLLER_H
#define HTTPREQUESTCONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"

class HttpRequestController : public HttpRequestHandler
{
    Q_OBJECT
public:
    HttpRequestController(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // HTTPREQUESTCONTROLLER_H
