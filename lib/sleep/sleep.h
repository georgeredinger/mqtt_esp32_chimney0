#ifndef _sleep_
#define _sleep_ true
//uint64_t sleepy_time = 1000000 * TIMECHUNK; //microseconds to sleep (deep_sleep)
#include <WiFi.h>

void gotosleep(uint64_t);
#endif
