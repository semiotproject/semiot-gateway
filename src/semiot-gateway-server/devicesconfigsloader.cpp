#include "devicesconfigsloader.h"
#include <QtQml>

DevicesConfigsLoader::DevicesConfigsLoader(bool debug, QObject *parent) : QObject(parent)
{
    qmlRegisterType(QUrl("qrc:/SemIoTDeviceConfig.qml"),"ru.semiot.gateway",0,1,"SemIoTDeviceConfig");
    //FIXME:
    udpDriver = new UDPDriver(debug);
    udpDriver->addDataSource(55555);
}

void DevicesConfigsLoader::addConfig(QUrl configUrl)
{
    // FIXME:
    QQmlEngine* engine = new QQmlEngine(this);
    QQmlComponent* component = new QQmlComponent(engine,configUrl);
    QObject *object = component->create();
    // TODO: connect if driver names are equal
    // qDebug()<<object->property("driverName");
    QObject::connect(udpDriver,SIGNAL(newDataReady(QVariant)),object,SIGNAL(newDataPacketReceived(QVariant)));
    QObject::connect(object,SIGNAL(newDataReady(QString,QString)),this,SIGNAL(newDataReady(QString,QString)));
}

