#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QUrl>
#include "httplistener.h"
#include "httprequestcontroller.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);

signals:

public slots:

private:
    HttpListener* _listener;
    QSettings* _settings;
};

#endif // HTTPSERVER_H
