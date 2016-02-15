import QtQuick 2.2
import ru.semiot.gateway 0.1

SemIoTDeviceConfig {
    // NOTE: is that ok that system driver is only one per device driver?
    driverName: "udp";
    property int listenPort: 33333
    onDriverConnected: {
        addDriverDataSource({"port":listenPort})
    }
    onNewDataPacketReceived: {
        // FORMAT:
        // "VLFR" (4B)
        // imp_counter (2B)
        // high_counter (4B)
        // MAC (6B)
        // == 16 BYTES
        // TODO: checksum?
        if (dataPacket.senderPort==listenPort) {
            //TODO: DEVICE_WORD check
            var tick = tickFromData(dataPacket.data)
            var highTick = highTickFromData(dataPacket.data)
            var realTick = tick+highTick*10
            var tick2Litre = tickCounter2Litre(realTick)
            deviceName = hashName("valtec-vlf-r-i"+"-"+driverName+"-"+dataPacket.senderHost+"-"+dataPacket.senderPort)

            var descriptionMap = {
                '\\${HOST}':dataPacket.senderHost,
                '\\${PORT}':dataPacket.senderPort
            };

            var electricEnergyConsumptionMap = {
                '\\${TIMESTAMP}':dataPacket.timeStamp,
                '\\${DATETIME}':dataPacket.dateTime,
                '\\${Tick}':realTick,
                '\\${tick2Litre}':tick2Litre
            };

            descriptionDesc = replaceAll(descriptionDescSrc, descriptionMap)
            electricEnergyConsumptionDesc = replaceAll(electricEnergyConsumptionDescSrc, electricEnergyConsumptionMap)

            // NOTE: is that ok that we decide here how to organize res pathes?
            newDataReady("/"+deviceName+"/description",descriptionDesc)
            newDataReady("/"+deviceName+"/waterConsumption",electricEnergyConsumptionDesc)
        }
    }

    property string deviceName

    property string descriptionDesc : ''
    property string electricEnergyConsumptionDesc : ''

    property string descriptionDescSrc : '
        This is ESP8266-Arduino based SemIoT device prototype for the
        Mercury-201 electric energy consumption counter.
        TODO: super ontology description.
        Device source: udp://${HOST}:${PORT}
    ';
    property string electricEnergyConsumptionDescSrc : '
        This is ESP8266-Arduino based SemIoT device prototype for the
        WATER consumption counter Valtec VLR-F-I.
        TODO: super ontology description.
        Tick value: ${Tick}.
        Litre value: ${tick2Litre}.
        Timestamp: ${TIMESTAMP}.
        Datetime: ${DATETIME}.
    ';

    // utils: NOTE: should probably move to udp driver
    function replaceAll(str,mapObj){
        var re = new RegExp(Object.keys(mapObj).join("|"),"g");

        return str.replace(re, function(matched){
            return mapObj[matched.replace(/[<>*()?$]/g, "\\$&")]; // :(
        });
    }

    function hashName(str) {
        var hash = 5381, // NOTE: why 5381
            i = str.length
        while(i)
          hash = (hash * 33) ^ str.charCodeAt(--i)
        return (hash >>> 0).toString();
    }

    function tickFromData(dataPacketData) {
        console.log("M201 data=",(dataPacketData[0]),dataPacketData[1],dataPacketData[2],dataPacketData[3],dataPacketData[4],dataPacketData[5],dataPacketData[6],dataPacketData[7],dataPacketData[8],dataPacketData[9],dataPacketData[10])
        return (dataPacketData[4]<<8)+dataPacketData[5]
    }
    function highTickFromData(dataPacketData) {
        return (dataPacketData[6]<<24)+(dataPacketData[7]<<16)+(dataPacketData[8]<<0)+dataPacketData[9]
    }
    function tickCounter2Litre(tick) {
        return tick*10
    }
}
