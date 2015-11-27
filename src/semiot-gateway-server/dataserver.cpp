#include "dataserver.h"

DataServer::DataServer()
{

}

QString DataServer::getValueByResourcePath(QString resourcePath)
{
    return _currentResourcesValues[resourcePath];
}
