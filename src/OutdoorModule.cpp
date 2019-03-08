#include "OutdoorModule.hpp"

OutdoorModule::OutdoorModule()
{
  dht=new DHT_Unified(D3, DHT11);
}

OutdoorModule::~OutdoorModule()
{
  delete dht;
}

bool OutdoorModule::readSensorValues()
{
  // Get temp and humidity values
  dht->begin();

  dht->temperature().getEvent(&event);
  temperature=event.temperature;

  dht->humidity().getEvent(&event);
  relative_humidity=event.relative_humidity;

  // Get daylight value
  daylight=0;

  for (int i=0; i<10; i++)
  {
    values[i]=analogRead(PIN_A0);
    daylight=daylight+values[i];
    delay(1000);  
  }

  daylight=daylight/10;  

  return true;

}

void OutdoorModule::publishValues() {

   if (!isnan(temperature)) {
    sprintf(stringValue, "%f", temperature); 
    Serial.println(stringValue);
    if(!publishMQTT("outdoor/temperature/value",  stringValue))    
      Serial.println("Error publishing to MQTT server.");
   }
   else
    Serial.println(F("Error reading temperature!"));
 
  if (!isnan(relative_humidity)) {
    sprintf(stringValue, "%f", relative_humidity); 
    Serial.println(stringValue);
    if(!publishMQTT("outdoor/humidity/value",  stringValue))    
      Serial.println("Error publishing to MQTT server.");
   }
   else
    Serial.println(F("Error reading humidity!"));

  
  sprintf(stringValue, "%d", daylight); 
   Serial.println(stringValue);
  if(!publishMQTT("outdoor/daylight/value",  stringValue))
  {
    Serial.println("Error publishing to MQTT server.");
    return;
  }

}

