#ifndef DEVICESCONFIGSLOADER_H
#define DEVICESCONFIGSLOADER_H

#include <QObject>
#include <QUrl>
#include "backmodule.h"
// as plugin
//#include "udpdriver.h"
#include <QtQml>

class DevicesConfigsLoader : public QObject
{
    Q_OBJECT
public:
    explicit DevicesConfigsLoader(QObject *parent = 0);

signals:
    void newDataReady(QString resourcePath, QString value);
    void driverConnected();
    void newRequestReceived(QVariant params);

public slots:
    void addConfig(QUrl configUrl); // NOTE: return config id?
    void continueLoading();

private:
    QQmlEngine* _engine;
    // TODO: modular system
    BackModule * _udpDriver; // TODO: drivers list
    // FIXME: save objects list:
    QObject * object;
    QQmlComponent* component;
};

#endif // DEVICESCONFIGSLOADER_H
