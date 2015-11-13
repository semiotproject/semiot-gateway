import QtQuick 2.2
import "./" as ConfigDir

Item {
    id: semIoTDeviceConfig
    property string driverName
    property var driverInitArgs
    signal newDataPacketReceived(var dataPacket)
    signal newDataReady(string path, string value)
}
