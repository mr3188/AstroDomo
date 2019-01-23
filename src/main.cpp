#include <Arduino.h>
#include <astrodomo.hpp>
#include <OutdoorModule.hpp>
#include <TempSensorsOW.hpp>

adbase *module=NULL;

void setup() {

  adbase *uninitializedModule= new adbase();
  
  
  switch ( uninitializedModule->moduleType())
  {
    case NODE_TYPE_TEMP_SENSORS_OW:
        module=new TempSensorsOW();
      break;
    case NODE_TYPE_OUTDOOR:
        module=new OutdoorModule();
      break;   
    default:
      Serial.println("Incorrect module type: "+uninitializedModule->moduleType());
      break;
  }
}

void loop() {
  module.readSensorValues();
  module.
}