#include "devicesconfigsloader.h"
#include "deviceconfig.h"

DevicesConfigsLoader::DevicesConfigsLoader(bool debug, QObject *parent) : QObject(parent)
{
    _engine = new QQmlEngine(this);
    qmlRegisterType<DeviceConfig>("ru.semiot.gateway",0,1,"SemIoTDeviceConfig");
    //FIXME:
    _udpDriver = new UDPDriver(debug);
}

void DevicesConfigsLoader::addConfig(QUrl configUrl)
{
    // FIXME: forgotten pointers
    QQmlComponent* component = new QQmlComponent(_engine,configUrl);
    QObject *object = component->create();
    // TODO: driversList:
    if (object->property("driverName")==_udpDriver->getDriverName()) {
        QObject::connect(_udpDriver,SIGNAL(newDataReady(QVariant)),object,SIGNAL(newDataPacketReceived(QVariant)));
        QObject::connect(object,SIGNAL(newDataReady(QString,QString)),this,SIGNAL(newDataReady(QString,QString)));
        QObject::connect(object,SIGNAL(addDriverDataSource(QVariant)),_udpDriver,SLOT(addDriverDataSource(QVariant)));
        DeviceConfig* device_object = dynamic_cast<DeviceConfig*>(object);
        if (true) {// TODO: if connect success
            emit device_object->driverConnected();
        }
    }
}

