#include "deviceconfig.h"

DeviceConfig::DeviceConfig(QObject *parent) : QObject(parent)
{

}

QString DeviceConfig::getDriverName()
{
    return _driverName;
}

void DeviceConfig::setDriverName(QString newValue)
{
    _driverName=newValue;
}

QVariant DeviceConfig::getDriverInitArgs()
{
    return _driverInitArgs;
}

void DeviceConfig::setDriverInitArgs(QVariant newValue)
{
    _driverInitArgs = newValue;
    emit driverInitArgsChanged();
}
