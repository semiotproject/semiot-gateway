#ifndef DEVICESCONFIGSLOADER_H
#define DEVICESCONFIGSLOADER_H

#include <QObject>
#include <QUrl>
// TODO: as plugin
#include "udpdriver.h"
#include <QtQml>

class DevicesConfigsLoader : public QObject
{
    Q_OBJECT
public:
    explicit DevicesConfigsLoader(bool debug = false, QObject *parent = 0);

signals:
    void newDataReady(QString resourcePath, QString value);
    void driverConnected();

public slots:
    void addConfig(QUrl configUrl); // NOTE: return config id?

private:
    QQmlEngine* _engine;
    // TODO: modular system
    UDPDriver * _udpDriver; // TODO: drivers list
};

#endif // DEVICESCONFIGSLOADER_H
