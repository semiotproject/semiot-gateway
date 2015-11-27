#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

/*
 * Minimal working version:
 *
 * Input (add driver) cfg: uri to ttl, driver name (1st version: preloaded?) and driver settings
 * We can additionaly edit all this cfg with qml file on the client
 *
 * After input we can recognize some devices (./protocol/device-unique-name/..)
 * Select them (optional, could be all) and after that the system should
 * parse some resources from devices (data sources) like: ./protocol/device-unique-name/resource-name
 *
 * After that it should be possible to locate to any device url,
 * get the list of the resources
 * and connect and warch any resource
 */

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QMultiHash>
#include <QVariant>
#include "dataserver.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(DataServer* dataServer, quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~WebSocketServer();
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void _onNewConnection();
    void _socketDisconnected();

private:
    QStringList _wellKnownCore;
    QWebSocketServer *_pWebSocketServer;
    QList<QWebSocket *> _clients; // TODO: QMultiHash
    bool _debug;
    DataServer* _dataServer;

signals:

public slots:
    void processNewData(QString resourcePath, QString value);
};

#endif // WEBSOCKETSERVER_H
