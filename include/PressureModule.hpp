
#ifndef OUTDOOR_MODULE_H
#define OUTDOOR_MODULE_H

#include "adbase.hpp"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


class PressureModule : public adbase
{
private:
    DHT_Unified *dht;
    char mqttTopic[128]; //"/pool/presure"
    int values[10];
    int pressure_value;
    char stringValue[20];

public:
    PressureModule();
    ~PressureModule();    
    bool readSensorValues();
    void publishValues();
    
};

#endif