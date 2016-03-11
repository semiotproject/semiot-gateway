#include "dataserver.h"
#include <QRegularExpression>
#include <QDebug>

DataServer::DataServer(QObject *parent) : QObject(parent)
{

}

QString DataServer::getResourcesList(QString system) const
{
    QStringList allResources(_currentResourcesValues.keys());
    if (system=="") {
        return allResources.join(";\n"); //TODO: json
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
        return resourcesList.join(";\n"); //TODO: json
    }
}

QString DataServer::getSystemsList() const
{
    QStringList systemsList;
    QStringList resourcesList = getResourcesList().split(";"); //fixme: json
    resourcesList.removeAll(WELLKNOWNCOREPATH);
    foreach (QString resource, resourcesList) {
        QString system = resource.split("/").first();
        if (!systemsList.contains(system)) {
            systemsList.append(system);
        }
    }
    return systemsList.join(";\n"); //TODO: json
}

QString DataServer::getValueByResourcePath(QString resourcePath) const
{
    // TODO:
    QRegularExpression systemRegExp;
    // api/systems
    systemRegExp.setPattern("^[/]api[/]systems[/]?$");
    systemRegExp.setPattern("^[/]api[/]systems[/]list[/]?$");

    // api/systems/.../system
    systemRegExp.setPattern("^[/]api[/]systems[/]system[/]?$");
    systemRegExp.setPattern("^[/]api[/]systems[/]system[/][0-9]+[/]?$");
    /*
    if (systemRegExp.match(resourcePath).hasMatch()) {
        return getResourcesList(resourcePath);
    }
    return _currentResourcesValues[resourcePath];
    */
    return QString("게이트웨이 왕!"); // TODO:
}

void DataServer::processNewData(QString resourcePath, QString value)
{
    _currentResourcesValues.insert(resourcePath,value);
}
