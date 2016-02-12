# SemIoT Gateway

![semiot_gateway_structure](https://github.com/semiotproject/semiot-gateway/raw/master/doc/semiot_gateway_structure.png)

This application allows you to share your local sensors observations in local or world-wide network. Observations could be annotated with semantic desctiptions to provide unified agile access for possible further complicated analysis and logical reasoning with another applications (like SemIoT Platform).

The SemIoT Gateway is Qt-based server that could annotate and share gathered sensors data via HTTP and WebSocket protocols.

Supported sensors protocols could be extended via binary plugins called "protocol drivers" (UDP protocol driver is already implemented as example).

Different IoT-devices could be supported via device drivers. Device driver is a js-based description of the device that could be loaded and configured in runtime. Device driver should use one of the avaliable protocol driver for getting and sending the data to the device. Device driver could contatin rules how to annotate sensor data with a semantic description.

There is also [SemIoT device prototype](https://github.com/semiotproject/semiot-device-prototype) repo which contants examples how to create simple device that could send the sensor (electricity, water consumption) data to local network.

## How to build SemIoT Gateway
    cd build
    qmake /.../src/semiot-gateway.pro
    make

## How to deploy
    run /.../build/semiot-gateway
    open http://<semiot_gateway_host>:<semiot_gateway_port>/ (default port: 8080)
    provide uri of the device driver, upload it
    configure driver (if necessary)
    enjoy

+ device driver example:
`/src/semiot-gateway-device-drivers/*.qml`
