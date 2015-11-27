#include "httpserver.h"
#include <QSettings>

HttpServer::HttpServer(DataServer *dataServer, QObject *parent) : QObject(parent), _dataServer(dataServer)
{
    _settings = new QSettings();
    _settings->beginGroup("listener");
    _settings->setValue(";host", "localhost");
    _settings->setValue("port", 8080);
    _settings->setValue("minThreads", 1);
    _settings->setValue("maxThreads", 100);
    _settings->setValue("cleanupInterval", 1000);
    _settings->setValue("readTimeout", 60000);
    _settings->setValue("maxRequestSize", 16000);
    _settings->setValue("maxMultiPartSize", 10000000);
    //_settings->endGroup();
    _listener = new HttpListener(_settings, new HttpRequestController(_dataServer,this), this);
}

