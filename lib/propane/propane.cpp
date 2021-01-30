#ifndef UNIT_TEST
#include <Arduino.h>
#define SENSOR 32
float getPropane(){
    float p;
    int ip;
    ip = analogRead(SENSOR);
    p = (float)ip;
    return p;
}
#endif