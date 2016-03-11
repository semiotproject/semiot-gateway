#ifndef FRONTMODULEMANAGER_H
#define FRONTMODULEMANAGER_H
#include <QObject>
#include "frontmodule.h"
#include "devicesconfigsloader.h"

class FrontModuleManager : public QObject
{
    Q_OBJECT
public:
    explicit FrontModuleManager(QObject *parent = 0);
    void connectFrontModule(FrontModule* fm);

signals:

public slots:
private:
    DevicesConfigsLoader* _cl;
    DataServer* _ds;
};

#endif // FRONTMODULEMANAGER_H
