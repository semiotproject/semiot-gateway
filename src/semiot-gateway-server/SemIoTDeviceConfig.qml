import QtQuick 2.2

Item {
    property string driverName
    property var driverInitArgs
    signal driverReady() // NOTE: send avaliable params?
    signal addDriverDataSource(var driverDataSource)
    signal newDataPacketReceived(var dataPacket)
    signal newDataReady(string path, string value)
}
