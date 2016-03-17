#include "dataserver.h"
#include <QRegularExpression>
#include <QDebug>
#include "filedownloader.h"
#include "redland.h"


DataServer::DataServer(QObject *parent) : QObject(parent)
{
    Dataquay::BasicStore store;
    store.setBaseUri(Dataquay::Uri("http://www.w3.org/ns/hydra/core#"));
    // Simple test. FIXME: remove
    QUrl testUrl("https://raw.githubusercontent.com/semiotproject/semiot-platform/master/api-gateway/src/main/resources/ru/semiot/platform/apigateway/ApiDocumentation.ttl");
    FileDownloader fd(testUrl);
    QString path = fd.download();
    store.import(path,Dataquay::BasicStore::ImportPermitDuplicates);
    Dataquay::Triple triple(Dataquay::Node(Dataquay::Uri(QUrl("http://semiot.ru/doc#EntryPoint-Sensors"))),Dataquay::Node(Dataquay::Uri(QUrl("http://www.w3.org/1999/02/22-rdf-syntax-ns#type"))),Dataquay::Node(Dataquay::Uri(QUrl("http://www.w3.org/ns/hydra/core#Link"))));
    store.save(QString("superstore.ttl"));
    qDebug()<<store.contains(triple);
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
