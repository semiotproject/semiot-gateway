#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "frontmodule.h"

//#include <QtCore/QObject>
#include <QList>
#include <QByteArray>
#include <QMultiHash>
//#include <QVariant>
//#include "dataserver.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public FrontModule
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = Q_NULLPTR);
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

signals:

public slots:
    void processNewData(QString resourcePath, QString value);
};

#endif // WEBSOCKETSERVER_H
