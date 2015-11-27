#ifndef DATASERVER_H
#define DATASERVER_H

#include <QString>
#include <QHash>

#define WELLKNOWNCOREPATH "/.well-known/core"

class DataServer
{
public:
    DataServer();
    //TODO: json
    QString getValueByResourcePath(QString resourcePath);
    QHash<QString,QString> _currentResourcesValues; // FIXME: to private
private:

};

#endif // DATASERVER_H
