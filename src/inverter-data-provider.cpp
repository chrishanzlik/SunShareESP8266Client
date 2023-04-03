#include "inverter-data-provider.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <Arduino.h>
#include <string>

InverterDataProvider::InverterDataProvider(String endpointUrl)
{
    url = endpointUrl;
}

InverterDataProvider::Response InverterDataProvider::provide()
{
    InverterDataProvider::Response response;
    response.message_only = false;

    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);

    int httpResponseCode = http.GET();
    if (httpResponseCode == 200)
    {
        String payload = http.getString();
        JSONVar myObject = JSON.parse(payload);

        if (JSON.typeof(myObject) == "undefined")
        {
            Serial.println("Parsing input failed!");
            response.message = "Parsing input failed!";
            response.message_only = true;

            return response;
        }
        if (JSON.typeof(myObject["message"]) == "string")
        {
            Serial.println(myObject["message"]);
            response.message = String((const char *)myObject["message"]);
            response.message_only = true;

            return response;
        }

        InverterData d;

        d.battery = myObject["snapshot"]["battery"];
        d.batteryLoad = myObject["snapshot"]["batteryLoad"];
        d.load = myObject["snapshot"]["load"];
        d.grid = myObject["snapshot"]["grid"];
        d.yield = myObject["snapshot"]["yield"];
        d.timestamp = String((const char *)myObject["timestamp"]);

        response.is_high_gain = (bool)myObject["isHighGain"];
        response.data = d;

        Serial.println(myObject);

        return response;
    }
    else
    {
        Serial.print("Error: ");
        Serial.println(httpResponseCode);
        if (httpResponseCode == 502)
        {
            response.message = "Bad Gateway (502)";
            response.message_only = true;

            return response;
        }
    }

    return response;
};