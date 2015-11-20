#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

#include <QObject>
#include <QVariant>

class DeviceConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString driverName READ getDriverName WRITE setDriverName NOTIFY driverNameChanged)
    Q_PROPERTY(QVariant driverInitArgs READ getDriverInitArgs WRITE setDriverInitArgs NOTIFY driverInitArgsChanged)

public:
    explicit DeviceConfig(QObject *parent = 0);

signals:
    void driverConnected();
    void addDriverDataSource(QVariant driverDataSource);
    void newDataPacketReceived(QVariant dataPacket);
    void newDataReady(QString path, QString value);
    void driverNameChanged();
    void driverInitArgsChanged();

public slots:
    QString getDriverName();
    void setDriverName(QString newValue);
    QVariant getDriverInitArgs();
    void setDriverInitArgs(QVariant newValue);

private:
    QString _driverName;
    QVariant _driverInitArgs;
};

#endif // DEVICECONFIG_H
