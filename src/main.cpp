#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "websocketserver.h"
#include "udpdriver.h" // TODO: load as plugin
#include <QtQml>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("SemIoT gateway");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    // TODO: get rid of magic default port:
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for gateway server [default: 123400]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("123400"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    WebSocketServer *server = new WebSocketServer(port, debug);
    QObject::connect(server, &WebSocketServer::closed, &a, &QCoreApplication::quit);
    // TODO: load as plugins:
    UDPDriver *udpDriver = new UDPDriver(debug);
    udpDriver->addDataSource(55555);

    // TODO: methods to load from url
    // declarative stuff:
    QQmlEngine engine;
    QQmlComponent component(&engine, "./config.qml");
    QObject *object = component.create();
    // TODO: connect if driver names are equal
    // qDebug()<<object->property("driverName");
    QObject::connect(udpDriver,SIGNAL(newDataReady(QVariant)),object,SIGNAL(newDataPacketReceived(QVariant)));
    QObject::connect(object,SIGNAL(newDataReady(QString,QString)),server,SLOT(processNewData(QString,QString)));
    return a.exec();
}

