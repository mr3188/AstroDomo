#ifndef TEMP_SENSORS_OW_H
#define TEMP_SENSORS_OW_H

#include "adbase.hpp"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <OneWire.h> 
#include <list>

struct addr
    {
      byte values[8];
    };

struct owSensor
  {
      addr address;
      float temp;
  };

class TempSensorsOW : public adbase
{
  private:
    uint8_t DS18S20_Pin = D2; //DS18S20 Signal pin on digital 2
    OneWire *ds;  // temperature sensors
    owSensor sensors[10];
    int devNumber=0;
    char stringValue[64];

  protected:


  public:
    TempSensorsOW();
    ~TempSensorsOW();
    void printAddress(owSensor sensor);
    void getDeviceAddress();    
    float  getTemp(owSensor sensor);    
    bool  readSensorValues();
    void publishValues();
};

#endif

