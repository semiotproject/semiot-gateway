// ESP8266-based module
// for incotex mercury 201 http://www.incotexcom.ru/m201.htm

import QtQuick 2.2
import ru.semiot.gateway 0.1

SemIoTDeviceConfig {
    // NOTE: is that ok that system driver is only one per device driver?
    driverName: "udp";
    property int listenPort: 55555
    onDriverConnected: {
        addDriverDataSource({"port":listenPort})
    }
    onNewDataPacketReceived: {
        // FORMAT:
        // "M201" (4B)
        // imp_counter (2B)
        // high_counter (4B)
        // newline (1B)
        // == 10 BYTES
        // TODO: checksum?
        // TODO: mac or some id
        if (dataPacket.senderPort==listenPort) {
            // TODO: DEVICE_WORD check
            var macAddr = macFromData(dataPacket.data)
            var tick = tickFromData(dataPacket.data)
            var kWhtick = kWhTickFromData(dataPacket.data)
            var realTick = tick+kWhtick*3200
            var tick2Wh = tickCounter2Wh(realTick)
            deviceName = "mercury-201-"+"-"+driverName+"-"+macAddr

            var descriptionMap = {
                '\\${MAC}':macAddr,
                '\\${HOST}':dataPacket.senderHost,
                '\\${PORT}':dataPacket.senderPort
            };

            var electricEnergyConsumptionMap = {
                '\\${TIMESTAMP}':dataPacket.timeStamp,
                '\\${DATETIME}':dataPacket.dateTime,
                '\\${Tick}':realTick,
                '\\${Tick2Wh}':tick2Wh
            };

            descriptionDesc = replaceAll(descriptionDescSrc, descriptionMap)
            electricEnergyConsumptionDesc = replaceAll(electricEnergyConsumptionDescSrc, electricEnergyConsumptionMap)

            // NOTE: is that ok that we decide here how to organize res pathes?
            newDataReady("/"+deviceName+"/description",descriptionDesc)
            newDataReady("/"+deviceName+"/electricEnergyConsumption",electricEnergyConsumptionDesc)
        }
    }

    property string deviceName

    property string descriptionDesc : ''
    property string electricEnergyConsumptionDesc : ''

    property string descriptionDescSrc : '
        This is ESP8266-Arduino based SemIoT device prototype for the
        Mercury-201 electric energy consumption counter.
        TODO: super ontology description.
        Device source: udp://${HOST}:${PORT}.
        Device MAC address: udp://${MAC}.
    ';
    property string electricEnergyConsumptionDescSrc : '
        This is ESP8266-Arduino based SemIoT device prototype for the
        Mercury-201 electric energy consumption counter.
        TODO: super ontology description.
        Tick value: ${Tick}.
        Watt-hour value: ${Tick2Wh} W*h.
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

    function macFromData(dataPacketData) {
        //console.log("M201 MAC data=",dataPacketData[10],dataPacketData[11],dataPacketData[12],dataPacketData[13],dataPacketData[14],dataPacketData[15])
        return dataPacketData[10].toString(16)+'-'+dataPacketData[11].toString(16)+'-'+dataPacketData[12].toString(16)+'-'+dataPacketData[13].toString(16)+'-'+dataPacketData[14].toString(16)+'-'+dataPacketData[15].toString(16)
    }
    function tickFromData(dataPacketData) {
        console.log("M201 data=",(dataPacketData[0]),dataPacketData[1],dataPacketData[2],dataPacketData[3],dataPacketData[4],dataPacketData[5],dataPacketData[6],dataPacketData[7],dataPacketData[8],dataPacketData[9],dataPacketData[10])
        return (dataPacketData[4]<<8)+dataPacketData[5]
    }
    function kWhTickFromData(dataPacketData) {
        return (dataPacketData[6]<<24)+(dataPacketData[7]<<16)+(dataPacketData[8]<<0)+dataPacketData[9]
    }
    function tickCounter2Wh(tick) {
        return tick*0.3125
    }
}
