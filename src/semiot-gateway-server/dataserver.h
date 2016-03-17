#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>
#include <QString>
#include <QHash>
#include "dataquay/Store.h"
#include "dataquay/BasicStore.h"

#define WELLKNOWNCOREPATH ".well-known/core"

class DataServer : public QObject
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
    QString getResourcesList(QString system = "") const;
    QString getSystemsList() const;
    // TODO: QVariant or something
    QString getValueByResourcePath(QString resourcePath) const;
    void processNewData(QString resourcePath, QString value);

private:
    QHash<QString,QString> _currentResourcesValues;
};

#endif // DATASERVER_H
