#ifndef DEVICESCONFIGSLOADER_H
#define DEVICESCONFIGSLOADER_H

#include <QObject>
#include <QUrl>
// TODO: as plugin
#include "udpdriver.h"

class DevicesConfigsLoader : public QObject
{
    Q_OBJECT
public:
    explicit DevicesConfigsLoader(bool debug = false, QObject *parent = 0);

signals:
    void newDataReady(QString resourcePath, QString value);

public slots:
    void addConfig(QUrl configUrl); // NOTE: return config id?

private:
    UDPDriver *udpDriver; // FIXME
};

#endif // DEVICESCONFIGSLOADER_H
