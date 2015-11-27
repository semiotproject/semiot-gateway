#include "dataserver.h"
#include <QDebug>

DataServer::DataServer()
{

}

QStringList DataServer::getResourcesList(QString system)
{
    if (system.startsWith("/")) {
        system.remove(0,1); // fixme
    }
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
        QString system = resource.split("/").first();
        if (!systemsList.contains(system)) {
            systemsList.append(system);
        }
    }
    return systemsList;
}

QString DataServer::getValueByResourcePath(QString resourcePath)
{
    if (resourcePath.startsWith("/")) {
        resourcePath.remove(0,1); // fixme
    }
    return _currentResourcesValues[resourcePath];
}
