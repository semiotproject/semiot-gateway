# SemIoT Gateway

linux-based gateway providing discovery and unified control
to all your home IoT-devices.

## Why:

+ discover IoT-devices in your home network (ethernet, wifi, bluetooth)
+ semantically describe your devices and make your house Smart (home automatization)
+ securely control your devices locally and via Internet
+ easily provide specified data to other users/companies
+ anything else you can imagine

This is kind of Smart Semantic IoT open-source router.

![Scheme](https://github.com/semiotproject/semiot-gateway/raw/master/scheme.png)

## Security
SSL

## Sparql Endpoint


```
SELECT ?resource
WHERE {
  ?resource a semiot-gateway:resource.
  ?resource semiot-gateway:gateway <gateway>.
}
```

```
SELECT ?user
WHERE {
  ?resource a semiot-gateway:resource.
  ?resource semiot-gateway:read_access <user>.
}
```

```
INSERT DATA
{
  <resource> a semiot-gateway:resource;
             semiot-gateway:gateway <gateway>.
}
```

```
INSERT DATA
{
  <resource> semiot-gateway:read_access <user_1>.
  <resource> semiot-gateway:read_write_access <user_2>.
}
```

## Controller

SemIoT Gateway should be controlled via [SemIoT Gateway Controller](https://github.com/semiotproject/semiot-gateway-controller)
