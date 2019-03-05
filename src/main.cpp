#include <Arduino.h>
#include <astrodomo.hpp>
#include <OutdoorModule.hpp>
#include <TempSensorsOW.hpp>

adbase *module=NULL;

void setup() {

  Serial.begin(9600);

  delay(1000);

  Serial.println("Starting...");

  adbase *uninitializedModule= new adbase();
  
  delay(5000);

  Serial.println("Module type ="+uninitializedModule->getModuleType());
  
  switch ( uninitializedModule->getModuleType())
  {
    case NODE_TYPE_TEMP_SENSORS_OW:
        module=new TempSensorsOW();
      break;
    case NODE_TYPE_OUTDOOR:
        module=new OutdoorModule();
      break;   
    default:
      Serial.println("Incorrect module type: "+uninitializedModule->getModuleType());
      break;
  }
}

void loop() {
  module->readSensorValues();
  module->publishValues();
  delay(module->getSleepTime());
}