#ifndef DATASERVER_H
#define DATASERVER_H

#include <QString>
#include <QHash>

#define WELLKNOWNCOREPATH "/.well-known/core"

class DataServer
{
public:
    DataServer();
    QString getResourcesList(QString system = "") const;
    QString getSystemsList() const;
    QString getValueByResourcePath(QString resourcePath) const;
    void processNewData(QString resourcePath, QString value);
private:
    QHash<QString,QString> _currentResourcesValues;
};

#endif // DATASERVER_H
