#ifndef UNIT_TEST
#include <Arduino.h>
#include "OTA.h"
#include "battery.h"
#include "mqtt.h"
#include "propane.h"
#include "sleep.h"
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <Wire.h>
#include <max6675.h>

int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

#define WDT_TIMEOUT 60    //Seconds
#define SLEEP_INTERVAL 5 //minutes


int print_wakeup_reason() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason) {
    case 1:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 3:
        Serial.println("Wakeup caused by timer");
        break;
    case 4:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 5:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.println("Wakeup was not caused by deep sleep");
        return (wakeup_reason);
    }
    return wakeup_reason;
}

void setup() {
    float ftemperature;
    char stemperature[20];
    Serial.begin(115200);
    Serial.println("MAX6675 test");
     esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               //add current thread to WDT watch
    setup_mqtt();

    ftemperature=thermocouple.readFahrenheit();
    sprintf(stemperature, "%2.2f", ftemperature);

    publish("chimney0/data", stemperature,5000);
    //publish("chimney0/batt", sbatt,5000);
    gotosleep(60 * SLEEP_INTERVAL); //minutes

}

void loop() {}
  
#endif