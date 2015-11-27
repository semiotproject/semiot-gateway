#ifndef DATASERVER_H
#define DATASERVER_H

#include <QString>
#include <QHash>


class DataServer
{
public:
    DataServer();
    QString getValueByResourcePath(QString resourcePath);
    QHash<QString,QString> _currentResourcesValues; // FIXME: to private
private:

};

#endif // DATASERVER_H
