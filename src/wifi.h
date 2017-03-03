//
// Created by Mike Mitterer on 18.02.17.
//

#ifndef ESP8266_TEST1_WIFI_H_H
#define ESP8266_TEST1_WIFI_H_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

// -- Passwords are invalid (2017-03-03)
const String ssid = "MikeMitterer-LS";
const String password = "q#83uouUhqCuhz";

//const String ssid = "MikeMitterer-TPL";
//const String password = "q083XruIh#hMfz";

const String host = "192.168.0.42";
const String url = "/api/dash/v1/hello";
const uint16_t port = 8080;

void setupWIFI(const String& ssid, const String& pw) {
    // Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    // would try to act as both a client and an access-point and could cause
    // network-issues with your other WiFi-devices on your WiFi-network.
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pw.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

/// Sendet den Request
/// Sample:
///     https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/client-examples.md
///
void sendGetRequest(const String& host, const String& url, const uint16_t port) {
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host.c_str(), port)) {
        Serial.println("connection failed");
        return;
    }

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    client.stop();
    Serial.println("\n[Disconnected]");
}


#endif //ESP8266_TEST1_WIFI_H_H
