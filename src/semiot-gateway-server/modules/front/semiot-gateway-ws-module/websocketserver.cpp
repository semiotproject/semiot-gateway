#include "websocketserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

QT_USE_NAMESPACE

WebSocketServer::WebSocketServer(quint16 port, QObject *parent) :
    FrontModule(parent),
    _pWebSocketServer(new QWebSocketServer(QStringLiteral("SemIoT Gateway"),QWebSocketServer::NonSecureMode, this)),
    _clients()
{
    if (_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSockets listening on port" << port;
        connect(_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::_onNewConnection);
        connect(_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }
}

WebSocketServer::~WebSocketServer()
{
    _pWebSocketServer->close();
    qDeleteAll(_clients.begin(), _clients.end());
}

void WebSocketServer::_onNewConnection()
{
    QWebSocket *pSocket = _pWebSocketServer->nextPendingConnection();
    qDebug() << "socketConnected:" << pSocket;
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::_socketDisconnected);

    _clients << pSocket;
    qDebug() << pSocket->requestUrl().path();
    QString path = pSocket->requestUrl().path();
    QString value = dataServer()->getValueByResourcePath(path);
    pSocket->sendTextMessage(value);
}

void WebSocketServer::_socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    // qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        _clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void WebSocketServer::processNewData(QString resourcePath, QString value)
{
    dataServer()->processNewData(resourcePath,value);

    foreach (QWebSocket* socket, _clients) {
        QString socketRequestUrlPath = socket->requestUrl().path();
        //qDebug() << "socketRequestUrlPath:" << socketRequestUrlPath;
        //qDebug() << "resourcePath:" << resourcePath;
        if (socketRequestUrlPath==resourcePath) {
            //qDebug() << "Sending to socket:" << socket << resourcePath << value;
            socket->sendTextMessage(value);
        }
    }
    if (!(_wellKnownCore.contains(resourcePath))) {
        _wellKnownCore.append(resourcePath);
        QString wellKnowCoreValue = _wellKnownCore.join(";\n");
        processNewData(WELLKNOWNCOREPATH,wellKnowCoreValue);
    }
}

