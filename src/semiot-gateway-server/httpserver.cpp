#include "httpserver.h"
#include <QSettings>

HttpServer::HttpServer(DataServer &dataServer, QObject *parent) : QObject(parent), _dataServer(dataServer)
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
    _requestController = new HttpRequestController(_dataServer,this);
    //TODO: set dataServer?
    connect(_requestController,SIGNAL(addDeviceDriverFromString(QString)),this,SIGNAL(addDeviceDriverFromString(QString)));
    connect(_requestController,SIGNAL(addDeviceDriverFromUrl(QUrl)),this,SIGNAL(addDeviceDriverFromUrl(QUrl)));
    connect(_requestController,SIGNAL(newRequestReceived(QVariant)),this,SIGNAL(newRequestReceived(QVariant)));
    _listener = new HttpListener(_settings, _requestController, this);
}

