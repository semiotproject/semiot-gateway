#ifndef FRONTMODULE_H
#define FRONTMODULE_H

#include <QObject>
#include <QUrl>
#include <QVariant>
#include "dataserver.h"

class FrontModule : public QObject
{
    Q_OBJECT
public:
    explicit FrontModule(QObject *parent = 0);

signals:
    // FIXME: unified method name:
    void addDeviceDriverFromUrl(QUrl url);
    void addDeviceDriverFromString(QString string);
    void newRequestReceived(QVariant params);

public slots:
    void setDataServer(DataServer *ds);
    DataServer* dataServer();

private:
    DataServer* _dataServer; // WHY NOT PUBLIC?

};

#endif // FRONTMODULE_H
