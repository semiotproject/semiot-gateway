#include "websocketserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

QT_USE_NAMESPACE

WebSocketServer::WebSocketServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    _pWebSocketServer(new QWebSocketServer(QStringLiteral("SemIoT Gateway"),QWebSocketServer::NonSecureMode, this)),
    _clients(),
    _debug(debug)
{
    if (_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (_debug)
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
    if (_debug)
        qDebug() << "socketConnected:" << pSocket;
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::_socketDisconnected);

    _clients << pSocket;
    if (_debug)
        qDebug() << pSocket->requestUrl().path();
    pSocket->sendTextMessage(getValueByResourcePath(pSocket->requestUrl().path()));
}

void WebSocketServer::_socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        _clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

QString WebSocketServer::getValueByResourcePath(QString resourcePath)
{
    if (_debug) {
        qDebug() << "getValueByResourcePath:"<<resourcePath
                 <<": "<<_currentResourcesValues.value(resourcePath);
        // qDebug() << _wellKnownCore.join(';');
    }
    return _currentResourcesValues[resourcePath];
}

void WebSocketServer::processNewData(QString resourcePath, QString value)
{
    if (_debug) {
        qDebug() << "processNewData resourcePath:" << resourcePath;
        qDebug() << "processNewData value:" << value;
    }
    _currentResourcesValues.insert(resourcePath,value);
    foreach (QWebSocket* socket, _clients) {
        QString socketRequestUrlPath = socket->requestUrl().path();
        if (_debug) {
            //qDebug() << "socketRequestUrlPath:" << socketRequestUrlPath;
            //qDebug() << "resourcePath:" << resourcePath;
        }
        if (socketRequestUrlPath==resourcePath) {
            if (_debug) {
                //qDebug() << "Sending to socket:" << socket << resourcePath << value;
            }
            socket->sendTextMessage(value);
        }
    }
    if (!(_wellKnownCore.contains(resourcePath))) {
        _wellKnownCore.append(resourcePath);
        QString wellKnowCoreValue = _wellKnownCore.join(';');
        processNewData(WELLKNOWNCOREPATH,wellKnowCoreValue);
    }
}

