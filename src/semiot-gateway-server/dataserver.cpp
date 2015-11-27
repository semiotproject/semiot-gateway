#include "dataserver.h"
#include <QDebug>

DataServer::DataServer()
{

}

QStringList DataServer::getResourcesList()
{
    return _currentResourcesValues.keys();
}

QStringList DataServer::getSystemsList()
{
    QStringList systemsList;
    QStringList resourcesList = getResourcesList();
    resourcesList.removeAll(WELLKNOWNCOREPATH);
    foreach (QString resource, resourcesList) {
        QString system = resource.split("/").at(1);
        if (!systemsList.contains(system)) {
            systemsList.append(system);
        }
    }
    return systemsList;
}

QString DataServer::getValueByResourcePath(QString resourcePath)
{
    return _currentResourcesValues[resourcePath];
}
