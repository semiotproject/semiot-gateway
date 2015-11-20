import QtQuick 2.2
import ru.semiot.gateway 0.1

SemIoTDeviceConfig {
    driverName: "udp";
    onDriverConnected: {
        addDriverDataSource({"port":55555})
    }
    onNewDataPacketReceived: {
        // PACKET FORMAT (15 bytes):
        // MAC -- 6 bytes
        // D -- 1 byte
        // H -- 4 bytes (float)
        // T -- 4 bytes (float)
        var mac = macFromData(dataPacket.data)
        var temperature = floatFromData(dataPacket.data,6+1+4)
        var humidity = floatFromData(dataPacket.data,6+1)
        // TODO: deviceNameHashGenerator
        deviceName = "dht11-"+mac+"-"+dataPacket.senderHost+"-"+dataPacket.senderPort
        //
        deviceName = hashName("dht11",mac,dataPacket.senderHost,dataPacket.senderPort)

        var descriptionMap = {
           '\\${MAC}':mac
        };

        var temperatureMap = {
            '\\${MAC}':mac,
            '\\${TIMESTAMP}':dataPacket.timeStamp,
            '\\${DATETIME}':dataPacket.dateTime,
            '\\${HOST}':dataPacket.senderHost,
            '\\${PORT}':dataPacket.senderPort,
            '\\${VALUE}':temperature
        };

        descriptionDesc = replaceAll(descriptionDescSrc, descriptionMap)
        temperatureDesc = replaceAll(temperatureDescSrc, temperatureMap)

        // NOTE: is that ok that we decide here how to organize ws pathes?
        newDataReady("/"+driverName+"/"+deviceName+"/description",descriptionDesc)
        newDataReady("/"+driverName+"/"+deviceName+"/temperature",temperatureDesc)
    }

    property string deviceName

    property string descriptionDesc : ''
    property string temperatureDesc : ''

    property string descriptionDescSrc : '
        @prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
        @prefix ssn: <http://purl.oclc.org/NET/ssnx/ssn#> .
        @prefix hmtr: <http://purl.org/NET/ssnext/heatmeters#> .
        @prefix ssncom: <http://purl.org/NET/ssnext/communication#> .

        <http://semiot.gateway/${MAC}> a hmtr:HeatMeter ;
            rdfs:label "Semiot device prototype #${MAC}"@en ;
            ssn:hasSubSystem <http://semiot.gateway/${MAC}/prototype> .

        <http://semiot.gateway/${MAC}/prototype>  a ssn:Sensor ;
            ssncom:hasCommunicationEndpoint <http://semiot.gateway/topic=${MAC}.prototype.obs> .

        <http://semiot.gateway/topic=${MAC}.prototype.obs> a ssncom:CommunicationEndpoint ;
            ssncom:protocol "WAMP" .
    ';
    property string temperatureDescSrc : '
    @prefix hmtr: <http://purl.org/NET/ssnext/heatmeters#> .
    @prefix meter: <http://purl.org/NET/ssnext/meters/core#> .
    @prefix ssn: <http://purl.oclc.org/NET/ssnx/ssn#> .
    @prefix xsd: <http://www.w3.org/2001/XMLSchema#> .
    @prefix : <http://semiot.gateway/${MAC}/prototype#> .

    :${TIMESTAMP} a hmtr:TemperatureObservation ;
        ssn:observationResultTime "${DATETIME}"^^xsd:dateTime ;
        ssn:observedBy <udp://${HOST}:${PORT}> ;
        ssn:observationResult :${TIMESTAMP}-result .

    :${TIMESTAMP}-result a hmtr:TemperatureSensorOutput ;
        ssn:isProducedBy <udp://${HOST}:${PORT}> ;
        ssn:hasValue :${TIMESTAMP}-resultvalue .

    :${TIMESTAMP}-resultvalue a hmtr:TemperatureValue ;
        meter:hasQuantityValue "${VALUE}"^^xsd:float ;
        om:hasQuantityUnitOfMeasurement <http://purl.oclc.org/NET/muo/ucum/unit/temperature/degree-Celsius> .
    ';

    // utils: NOTE: should probably move to udp driver
    function replaceAll(str,mapObj){
        var re = new RegExp(Object.keys(mapObj).join("|"),"g");

        return str.replace(re, function(matched){
            return mapObj[matched.replace(/[<>*()?$]/g, "\\$&")]; // :(
        });
    }

    function hashName(modelName,macAddr,host,port) {
        var str = modelName+macAddr+host+port
        var hash = 5381,
            i = modelName.length
        while(i)
          hash = (hash * 33) ^ modelName.charCodeAt(--i)
        return (hash >>> 0).toString();
    }

    function byteToHex(b) {
        var hexChar = ["0", "1", "2", "3", "4", "5", "6", "7","8", "9", "A", "B", "C", "D", "E", "F"];
        return hexChar[(b >> 4) & 0x0f] + hexChar[b & 0x0f];
    }
    function macFromData(dataPacketData) {
        var output = ''
        for (var i = 0; i<6; i++) {
            output += byteToHex(dataPacketData[i])
            output += ':'
        }
        output=output.slice(1, -1)
        return output
    }

    function floatFromData(dataPacketData, delta) {
        var bytesArray = []
        for (var i = 0; i<4; i++) {
            bytesArray.push(dataPacketData[i+delta]);
        }
        return decodeFloat(bytesArray, 1, 8, 23, -126, 127, true)
    }
    // Derived from http://stackoverflow.com/a/8545403/106786
    function decodeFloat(bytes, signBits, exponentBits, fractionBits, eMin, eMax, littleEndian) {

        var totalBits = (signBits + exponentBits + fractionBits);

        var binary = "";
        for (var i = 0, l = bytes.length; i < l; i++) {
          var bits = bytes[i].toString(2);
          while (bits.length < 8)
            bits = "0" + bits;

          if (littleEndian)
            binary = bits + binary;
          else
            binary += bits;
        }

        var sign = (binary.charAt(0) == '1')?-1:1;
        var exponent = parseInt(binary.substr(signBits, exponentBits), 2) - eMax;
        var significandBase = binary.substr(signBits + exponentBits, fractionBits);
        var significandBin = '1'+significandBase;
        var i = 0;
        var val = 1;
        var significand = 0;

        if (exponent == -eMax) {
            if (significandBase.indexOf('1') == -1)
                return 0;
            else {
                exponent = eMin;
                significandBin = '0'+significandBase;
            }
        }

        while (i < significandBin.length) {
            significand += val * parseInt(significandBin.charAt(i));
            val = val / 2;
            i++;
        }

        return sign * significand * Math.pow(2, exponent);
    }
}
