#ifndef UDPDRIVER_H
#define UDPDRIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QVariantMap>
#include <QDateTime>

class UDPDriver : public QObject
{
    Q_OBJECT
public:
    explicit UDPDriver(bool debug = false, QObject *parent = 0);

signals:
    void newDataReady(QVariant value);

public slots:
    void discoverDataSources(int port); // TODO: string config
    void addDataSource(quint16 port, const QHostAddress & address = QHostAddress::Any);

private slots:
    void readPendingDatagrams();

private:
    QString _driverName = "udp"; // TODO: qproperty
    QList<QUdpSocket*> _sockets;
    void addSocket(quint16 port, const QHostAddress & address);
    void processTheDatagram(QByteArray *datagram, quint16 port, QHostAddress *address);
    bool _debug;
};

#endif // UDPDRIVER_H
