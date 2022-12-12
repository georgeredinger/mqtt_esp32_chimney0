#ifndef UNIT_TEST
#include "Adafruit_MCP9600.h"
#include "OTA.h"
#include "battery.h"
#include "mqtt.h"
#include "propane.h"
#include "sleep.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <Wire.h>
#include <esp_system.h>
#include <esp_task_wdt.h>

const int ref2048_measure = 32;
const int ref2048_vcc = 33;
const int ref2048_gnd = 27;

#define WDT_TIMEOUT 10   //Seconds
#define SLEEP_INTERVAL 5 //minutes
RTC_DATA_ATTR float lastTH = 0.0;
RTC_DATA_ATTR int testCycles = 0;

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

#define I2C_ADDRESS (0x67)

#define WDT_TIMEOUT 9
  
Adafruit_MCP9600 mcp;

float batteryVoltage(){
    int ref;
    float fref;
    float voltsPerCount;
    const float fudge=0.959;
    pinMode(ref2048_measure, INPUT);
    pinMode(ref2048_vcc, OUTPUT);
    pinMode(ref2048_gnd, OUTPUT);
    digitalWrite(ref2048_gnd,0);
    digitalWrite(ref2048_vcc,1);
    ref=analogRead(ref2048_measure);
    fref = (float)ref;
    voltsPerCount = 2.048/fref;
    return((voltsPerCount*4096.0)*fudge);
}

void setup() {
    char json[40];
    
    float tc,th,b;
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch

    Serial.begin(115200);
    delay(250);
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               //add current thread to WDT watch
  
    Wire.begin(21, 22);
    if (!mcp.begin(I2C_ADDRESS)) {
        Serial.println("Sensor not found. Check wiring!");
        ESP.restart();
    }
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.enable(true);
    
    th = mcp.readThermocouple();
    tc =  mcp.readAmbient();
    b = batteryVoltage();

    if((testCycles++ >=60) || abs(th - lastTH) > 5){ //publish one per hour or when big TH changes
        testCycles =0;
        lastTH=th;
        setup_mqtt();
        sprintf(json, "{\"th\":%2.2f,\"tc\":%2.2f,\"b\":%2.2f}", th, tc, b);
        publish("chimney0/json", json, 5000);
    }
    gotosleep(60);
}

void loop() {
}
#endif
