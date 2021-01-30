#ifndef UNIT_TEST

#include <Arduino.h>
float getBattery() {
    pinMode(34, INPUT);
    int batt = analogRead(34);
    return (3.3 * ((float)batt / 4096) * 2 * (4.0 / 3.77)); //empirical cal
}
#endif