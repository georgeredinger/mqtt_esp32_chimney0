#ifndef UNIT_TEST
#include "OTA.h"
#include "battery.h"
#include "mqtt.h"
#include "propane.h"
#include "sleep.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <Wire.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <max6675.h>

const int led = 13;
const int ref2048 = 34;
const int thermoDO = 19;
const int thermoCS = 23;
const int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

#define WDT_TIMEOUT 10   //Seconds
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
    char sbatteryVoltage[20];
    int ref;
    float fref;
    float voltsPerCount;
    float batteryVoltage;

    Serial.begin(115200);
    Serial.println("MAX6675 test");
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               //add current thread to WDT watch
    pinMode(ref2048,INPUT);
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    ref = analogRead(ref2048);
    fref = (float) ref;
    voltsPerCount = 2.048/fref;
    batteryVoltage = (voltsPerCount*4096.0)*(3.329/3.58);//with emperical cal
    ftemperature = thermocouple.readFahrenheit();


    setup_mqtt();

    sprintf(stemperature, "%2.2f", ftemperature);
    sprintf(sbatteryVoltage, "%2.2f", batteryVoltage);

    publish("chimney0/data", stemperature, 0);
    publish("chimney0/battery", sbatteryVoltage, 5000);
    gotosleep(60 * SLEEP_INTERVAL); //minutes
}

void loop() {
    float ftemperature = thermocouple.readFahrenheit();
    int ref;
    float fref;
    float voltsPerCount;
    float batteryVoltage;

    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    ref = analogRead(ref2048);
    fref = (float) ref;
    voltsPerCount = 2.048/fref;
    batteryVoltage = voltsPerCount*4096.0;
    Serial.printf("%f,%f,%f,%f\r\n",ftemperature,fref,voltsPerCount,batteryVoltage);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    esp_task_wdt_reset();
}

#endif