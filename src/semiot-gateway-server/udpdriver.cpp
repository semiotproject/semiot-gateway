#include "udpdriver.h"
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QJSValue>
#include <QByteArray>

UDPDriver::UDPDriver(bool debug, QObject *parent) : QObject(parent),
  _debug(debug)
{
    _txSocket = new QUdpSocket(this);
}

void UDPDriver::discoverDataSources(int port)
{

}

void UDPDriver::addDataSource(quint16 port, const QHostAddress & address = QHostAddress::Any)
{
    return addSocket(port,address);
}

void UDPDriver::addDriverDataSource(QVariant params)
{
    bool ok;
    auto jsv = params.value<QJSValue>(); // FIXME
    addDataSource(jsv.property("port").toUInt());
}

QString UDPDriver::getDriverName()
{
    return _driverName;
}

void UDPDriver::actuate(QVariant params)
{
    auto jsv = params.value<QJSValue>(); // FIXME
    uint port = jsv.property("port").toUInt();
    QString address = jsv.property("host").toString();
    QByteArray data = jsv.property("data").toString().toUtf8();

    sendData(data,port,QHostAddress(address));
}

void UDPDriver::readPendingDatagrams()
{
    QUdpSocket* udpSocket = dynamic_cast<QUdpSocket*>(sender());
    if( udpSocket != NULL ) {
        while (udpSocket->hasPendingDatagrams()) {
            QByteArray* datagram = new QByteArray; // will be deleted after processing
            datagram->resize(udpSocket->pendingDatagramSize());
            QHostAddress* senderHost = new QHostAddress(); // will be deleted after processing
            quint16 senderPort;

            udpSocket->readDatagram(datagram->data(), datagram->size(),senderHost, &senderPort);
            qDebug()<<"datagram: "<<*datagram;
            qDebug()<<"port: "<<senderPort;
            qDebug()<<"address: "<<senderHost;

            if (QString::fromLatin1(datagram->data())==QString(UDP_GTW_PING)) {
                qDebug()<<"answering ping";
                udpSocket->writeDatagram(QByteArray(UDP_GTW_OK,UDP_GTW_OK_SIZE), *senderHost, senderPort);
            }
            else {
                processTheDatagram(datagram,senderPort,senderHost);
            }
        }
    }
}

void UDPDriver::addSocket(quint16 port, const QHostAddress & address = QHostAddress::Any)
{
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->bind(port); // FIXME: address support

    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    _sockets << udpSocket;
}

void UDPDriver::processTheDatagram(QByteArray * datagram, quint16 port, QHostAddress * address)
{
    // http://stackoverflow.com/questions/30668001/what-does-qbytearray-get-converted-to-in-qml
    QVariantList data;
    foreach (u_int8_t byte, *datagram) {
        data.append(byte);
    }
    if (_debug) {
        // qDebug()<<"datagram: "<<*datagram;
        // qDebug()<<"port: "<<port;
        // qDebug()<<"address: "<<address;
    }
    QVariantMap map; // TODO: json?

    map.insert("data", data);
    map.insert("timeStamp", QDateTime::currentDateTime().toTime_t());
    map.insert("dateTime", QDateTime::currentDateTime().toTimeSpec(Qt::UTC).toString());
    map.insert("senderHost", address->toString()); // NOTE: QString, rly?
    map.insert("senderPort", port);
    // FIXME: bad design, rewrite to send the objects via signals:
    delete datagram;
    delete address;
    if (_debug) {
        // qDebug()<<map;
    }
    emit newDataReady(map);
}

void UDPDriver::sendData(QByteArray datagram, quint16 port, QHostAddress address)
{
    _txSocket->writeDatagram(datagram, datagram.size(), address, port);
}
