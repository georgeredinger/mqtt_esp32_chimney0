#ifndef UNIT_TEST

#include "sleep.h"
#include <PubSubClient.h>
#include <WiFi.h>
const char *ID = "propane";                  // Name of our device, must be unique
const char *TOPIC_DATA = "propane/data";     // Topic to subcribe to
const char *TOPIC_RESETS = "propane/resets"; // Topic to subcribe to
const char *TOPIC_ALIVE = "propane/alive";   // Topic to subcribe to
const char *ssid = "unbalanced";               // name of your WiFi network
const char *password = "leapyear";           // password of the WiFi network
const char *mqttUser = "mosc";
const char *mqttPassword = "ito";
IPAddress broker(192, 168, 2, 11);          // IP address of your MQTT broker eg. 192.168.1.50
WiFiClient wclient;

PubSubClient client(wclient); // Setup MQTT client

// Connect to WiFi network
void setup_wifi() {
    int i = 10;
    WiFi.begin(ssid, password);             // Connect to network
    while (WiFi.status() != WL_CONNECTED) { // Wait for connection
        delay(1500);
        Serial.printf("WiFi.status != WL_CONNECTED %d\r\n", i);
        if (i-- <= 0)
            gotosleep(3600); //1 hour sleep if cannot connect
    }
}

// Reconnect to client
void reconnect() {
    int i = 10;

    // Loop until we're reconnected
    while (!client.connected()) {
        // Attempt to connect
        if (client.connect(ID,mqttUser,mqttPassword)) {
            Serial.printf("mqtt reconnect OK %d\r\n", i);
            ;
        } else {
            // Wait 5 seconds before retrying
            Serial.printf("mqtt reconnect %d\r\n", i);
            delay(5000);
            if (i-- <= 0)
                gotosleep(3600); //1 hour sleep if cannot connect
        }
    }
}

void setup_mqtt() {
    setup_wifi(); // Connect to network
    client.setServer(broker, 1883);
}

void publish(const char *topic, const char *data, int del) {
    if (!client.connected()) { // Reconnect if connection is lost
        reconnect();
    }
    client.loop();
    client.publish(topic, data);
    Serial.printf("publish %s %s %d %jd\r\n",topic,data,del,esp_timer_get_time()/1000000);
    delay(del); //last call delays for completion before sleep
}
#endif