#ifndef UDPDRIVER_H
#define UDPDRIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QDateTime>

#define UDP_GTW_OK "GTWOK"
#define UDP_GTW_OK_SIZE 5
#define UDP_GTW_PING "GTW"

class UDPDriver : public QObject
{
    Q_OBJECT
public:
    explicit UDPDriver(bool debug = false, QObject *parent = 0);

signals:
    void newDataReady(QVariant value);

public slots:
    void discoverDataSources(int port); // TODO: string config
    void addDataSource(quint16 port, const QHostAddress &address);
    void addDriverDataSource(QVariant params);
    QString getDriverName();
    void actuate(QVariant params);

private slots:
    void readPendingDatagrams();

private:
    QString _driverName = "udp"; // TODO: qproperty
    QList<QUdpSocket*> _sockets;
    void addSocket(quint16 port, const QHostAddress & address);
    void processTheDatagram(QByteArray *datagram, quint16 port, QHostAddress *address);
    void sendData(QByteArray datagram, quint16 port, QHostAddress address);
    bool _debug;
    QUdpSocket* _txSocket;
};

#endif // UDPDRIVER_H
