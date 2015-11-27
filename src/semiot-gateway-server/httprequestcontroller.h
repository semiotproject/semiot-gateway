#ifndef HTTPREQUESTCONTROLLER_H
#define HTTPREQUESTCONTROLLER_H

//#include <QObject>
#include "httprequesthandler.h"
#include "dataserver.h"

class HttpRequestController : public HttpRequestHandler
{
    //Q_OBJECT
public:
    HttpRequestController(DataServer* dataServer, QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
private:
    DataServer* _dataServer;
};

#endif // HTTPREQUESTCONTROLLER_H
