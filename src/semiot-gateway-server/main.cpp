#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtQml>
#include <QQmlEngine>
#include <QDebug>
#include "websocketserver.h"
#include "devicesconfigsloader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("SemIoT gateway");
    parser.addHelpOption();

    // TODO: miltiple devices

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    // TODO: get rid of magic default port:
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for gateway server [default: 57864]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("57864"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    WebSocketServer *server = new WebSocketServer(port, debug);
    QObject::connect(server, &WebSocketServer::closed, &a, &QCoreApplication::quit);

    DevicesConfigsLoader devicesConfigsLoader;

    QObject::connect(&devicesConfigsLoader,SIGNAL(newDataReady(QString,QString)),server,SLOT(processNewData(QString,QString)));
    // FIXME:
    devicesConfigsLoader.addConfig(QUrl::fromLocalFile("/home/a_andreyev/code/semiot/semiot-gateway/src/config.qml"));
    return a.exec();
}

