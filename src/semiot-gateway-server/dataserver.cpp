#include "dataserver.h"
#include <QDebug>

DataServer::DataServer()
{

}

QStringList DataServer::getResourcesList(QString system)
{
    QStringList allResources(_currentResourcesValues.keys());
    if (system=="") {
        return allResources;
    }
    else {
        QStringList resourcesList;
        QString resource;
        for (int i=0;i<allResources.size();i++) {
            resource = allResources.at(i);
            if (resource.startsWith(system+"/")) {
                resourcesList.append(resource);
            }
        }
        return resourcesList;
    }
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
