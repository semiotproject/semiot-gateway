#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtQml>
#include <QQmlEngine>
#include <QDebug>
#include "websocketserver.h"
#include "frontmodulemanager.h"
#include "httpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // TODO: separate to module based on Server Configurator App
    // with options to turn on/off components/etc
    QCommandLineParser parser;
    parser.setApplicationDescription("SemIoT gateway");
    parser.addHelpOption();
    // TODO: miltiple devices
    QCommandLineOption dbgOption(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    // TODO: get rid of magic default port:
    QCommandLineOption portOption(QStringList() << "p" << "port", QCoreApplication::translate("main", "Port for gateway server [default: 57864]."), QCoreApplication::translate("main", "port"), QLatin1Literal("57864"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    Q_UNUSED(debug); //TODO
    int port = parser.value(portOption).toInt();
    // Internal part
    //DataServer dataServer;
    //DevicesConfigsLoader devicesConfigsLoader;
    FrontModuleManager fMM(&a);
    // External Part
    WebSocketServer wsServer(port,&a);
    HttpServer httpServer(&a);
    fMM.connectFrontModule((FrontModule*)&wsServer);
    fMM.connectFrontModule((FrontModule*)&httpServer);
    //wsServer.setDataServer(&dataServer);
    //httpServer.setDataServer(&dataServer);
    // FIXME: for all the protocols
    /*
    QObject::connect(&devicesConfigsLoader,SIGNAL(newDataReady(QString,QString)),&wsServer,SLOT(processNewData(QString,QString)));
    QObject::connect(&httpServer,SIGNAL(newRequestReceived(QVariant)),&devicesConfigsLoader,SIGNAL(newRequestReceived(QVariant)));
    QObject::connect(&httpServer,SIGNAL(addDeviceDriverFromUrl(QUrl)),&devicesConfigsLoader,SLOT(addConfig(QUrl)));
    */
    // TODO: linux kernel driver
    // TODO: dbus
    // TODO: systemd?
    return a.exec();
}

