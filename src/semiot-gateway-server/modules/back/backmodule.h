#ifndef BACKMODULE_H
#define BACKMODULE_H

#include <QObject>
#include <QVariant>

#define UDP_GTW_OK "GTWOK"
#define UDP_GTW_OK_SIZE 5
#define UDP_GTW_PING "GTW"

class BackModule : public QObject
{
    Q_OBJECT
public:
    explicit BackModule(QObject *parent = 0);

signals:
    void newDataReady(QVariant value);

public slots:
    void discoverDataSources(QVariant params);
    void addDriverDataSource(QVariant params);
    QString getModuleName();
    void actuate(QVariant params);
};

#endif // BACKMODULE_H
