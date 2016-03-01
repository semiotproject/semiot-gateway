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
    component = new QQmlComponent(_engine,configUrl);
    if (component->isLoading()) {
        QObject::connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(continueLoading()));
    }
    else {
        continueLoading();
    }
}

void DevicesConfigsLoader::continueLoading()
{
    if (component->isError()) {
        qWarning() << component->errors();
    } else {
        object = component->create();
        // TODO: driversList:
        if (object->property("driverName")==_udpDriver->getDriverName()) {
            QObject::connect(_udpDriver,SIGNAL(newDataReady(QVariant)),object,SIGNAL(newDataPacketReceived(QVariant)));
            QObject::connect(this,SIGNAL(newRequestReceived(QVariant)),object,SIGNAL(newRequestReceived(QVariant)));
            QObject::connect(object,SIGNAL(newDataReady(QString,QString)),this,SIGNAL(newDataReady(QString,QString)));
            QObject::connect(object,SIGNAL(addDriverDataSource(QVariant)),_udpDriver,SLOT(addDriverDataSource(QVariant)));
            QObject::connect(object,SIGNAL(actuate(QVariant)),_udpDriver,SLOT(actuate(QVariant)));
            DeviceConfig* device_object = dynamic_cast<DeviceConfig*>(object);
            if (true) {// TODO: if connect success
                emit device_object->driverConnected();
            }
        }
    }
}

