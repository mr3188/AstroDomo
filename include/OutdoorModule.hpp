
#ifndef OUTDOOR_MODULE_H
#define OUTDOOR_MODULE_H

#include "adbase.hpp"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


class OutdoorModule : public adbase
{
private:
    DHT_Unified *dht;

    char mqttTopic[128]; //"/outdoor/daylight/value"
    int values[10];
    int daylight=0;
    char stringValue[10];
    sensors_event_t event;  


public:
    OutdoorModule();
    ~OutdoorModule();    
    void readSensorValues();
    void publishValues();
    
};

#endif