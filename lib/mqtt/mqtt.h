#ifndef __MQTT__
#define __MQTT__
void reconnect() ;
void publish(const char *, const char *,int) ;
void setup_mqtt() ;
extern const char *ID ;  // Name of our device, must be unique
extern const char *TOPIC_DATA ;  // Topic to subcribe to
extern const char *TOPIC_RESETS ;  // Topic to subcribe to
extern const char *TOPIC_ALIVE;
#endif