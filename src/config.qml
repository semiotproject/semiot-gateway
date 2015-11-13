import QtQuick 2.2
import "./" as ConfigDir // register as custom library

ConfigDir.SemIoTDeviceConfig {
    driverName: "udp";
    driverInitArgs: {"port": 55555}
    property string deviceName
    onNewDataPacketReceived: {
        // PACKET FORMAT (15 bytes):
        // MAC -- 6 bytes
        // D -- 1 byte
        // H -- 4 bytes (float)
        // T -- 4 bytes (float)
        var mac = macFromData(dataPacket.data)
        var temperature = floatFromData(dataPacket.data,6+1+4)
        var humidity = floatFromData(dataPacket.data,6+1)
        deviceName = "dht11-"+mac+"-"+dataPacket.senderHost+"-"+dataPacket.senderPort

        descriptionDesc = descriptionDesc.replace(/\${MAC}/g, mac)
        temperatureDesc = temperatureDesc.replace(/\${TIMESTAMP}/g, dataPacket.timeStamp)
        temperatureDesc = temperatureDesc.replace(/\${DATETIME}/g, dataPacket.dateTime)
        temperatureDesc = temperatureDesc.replace(/\${HOST}/g, dataPacket.senderHost)
        temperatureDesc = temperatureDesc.replace(/\${PORT}/g, dataPacket.senderPort)
        temperatureDesc = temperatureDesc.replace(/\${VALUE}/g, temperature)
        // NOTE: is that ok that we decide here how to organize ws pathes?
        newDataReady(deviceName+"/description",descriptionDesc)
        newDataReady(deviceName+"/temperature",temperatureDesc)
    }

    property string descriptionDesc : '
        @prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
        @prefix ssn: <http://purl.oclc.org/NET/ssnx/ssn#> .
        @prefix hmtr: <http://purl.org/NET/ssnext/heatmeters#> .
        @prefix ssncom: <http://purl.org/NET/ssnext/communication#> .

        <http://prototype.com/${MAC}> a hmtr:HeatMeter ;
            rdfs:label "Semiot device prototype #${MAC}"@en ;
            ssn:hasSubSystem <http://prototype.com/${MAC}/prototype> .

        <http://prototype.com/${MAC}/prototype>  a ssn:Sensor ;
            ssncom:hasCommunicationEndpoint <http://prototype.com/topic=${MAC}.prototype.obs> .

        <http://prototype.com/topic=${MAC}.prototype.obs> a ssncom:CommunicationEndpoint ;
            ssncom:protocol "WAMP" .
    ';
    property string temperatureDesc : '
    @prefix hmtr: <http://purl.org/NET/ssnext/heatmeters#> .
    @prefix meter: <http://purl.org/NET/ssnext/meters/core#> .
    @prefix ssn: <http://purl.oclc.org/NET/ssnx/ssn#> .
    @prefix xsd: <http://www.w3.org/2001/XMLSchema#> .
    @prefix : <http://prototype.com/${MAC}/prototype#> .

    :${TIMESTAMP} a hmtr:TemperatureObservation ;
        ssn:observationResultTime "${DATETIME}"^^xsd:dateTime ;
        ssn:observedBy <coap://${HOST}:${PORT}/meter> ;
        ssn:observationResult :${TIMESTAMP}-result .

    :${TIMESTAMP}-result a hmtr:TemperatureSensorOutput ;
        ssn:isProducedBy <coap://${HOST}:${PORT}/meter> ;
        ssn:hasValue :${TIMESTAMP}-resultvalue .

    :${TIMESTAMP}-resultvalue a hmtr:TemperatureValue ;
        meter:hasQuantityValue "${VALUE}"^^xsd:float ;
        om:hasQuantityUnitOfMeasurement <http://purl.oclc.org/NET/muo/ucum/unit/temperature/degree-Celsius> .
    ';

    // utils: NOTE: should probably move to udp driver
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
