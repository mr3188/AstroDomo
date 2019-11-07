#include "PressureModule.hpp"

PressureModule::PressureModule()
{
  
}

PressureModule::~PressureModule()
{
 
}

bool PressureModule::readSensorValues()
{
 

  for (int i=0; i<10; i++)
  {
    values[i]=analogRead(PIN_A0);
    pressure_value=pressure_value+values[i];
    delay(1000);  
  }

  pressure_value=pressure_value/10;  

  return true;

}

void PressureModule::publishValues() {

  sprintf(stringValue, "%d", pressure_value); 
   Serial.println(stringValue);
  if(!publishMQTT("pool/pressure",  stringValue))
  {
    Serial.println("Error publishing to MQTT server.");
    return;
  }

}


