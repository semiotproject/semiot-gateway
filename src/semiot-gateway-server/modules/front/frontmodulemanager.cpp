#include "frontmodulemanager.h"

FrontModuleManager::FrontModuleManager(QObject *parent) : QObject(parent)
{
    _cl = new DevicesConfigsLoader(this);
    _ds = new DataServer(this);
}

void FrontModuleManager::connectFrontModule(FrontModule *fm)
{
    fm->setDataServer(_ds);
    QObject::connect(_cl,SIGNAL(newDataReady(QString,QString)),
                     fm,SLOT(processNewData(QString,QString)));
    QObject::connect(fm,SIGNAL(newRequestReceived(QVariant)),
                     _cl,SIGNAL(newRequestReceived(QVariant)));
    QObject::connect(fm,SIGNAL(addDeviceDriverFromUrl(QUrl)),
                     _cl,SLOT(addConfig(QUrl)));
}
