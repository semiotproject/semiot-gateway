# encoding: utf-8
require "net/http"
require "uri"

uri = URI.parse("http://localhost:8080/api/register_device_driver")
response = Net::HTTP.post_form(uri, {"type" => "url", "data" => "https://raw.githubusercontent.com/semiotproject/semiot-gateway/master/src/config.qml"})
puts response
