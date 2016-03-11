#include "frontmodule.h"

FrontModule::FrontModule(QObject *parent) : QObject(parent)
{
}

void FrontModule::setDataServer(DataServer * ds)
{
    _dataServer = ds;
}

DataServer *FrontModule::dataServer()
{
    return _dataServer;
}
