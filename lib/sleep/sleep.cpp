#ifndef UNIT_TEST
#include <esp_task_wdt.h>

#include "sleep.h"
#define FIVE 13 // 5V boost enable

void gotosleep(uint64_t seconds) {
    digitalWrite(FIVE, LOW); //turn off 5V
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.printf("gotosleep() %d %jd\r\n", seconds, esp_timer_get_time() / 1000000);

    esp_sleep_enable_timer_wakeup(seconds * 1000000); //microseconds
    esp_deep_sleep_start();
}

#endif