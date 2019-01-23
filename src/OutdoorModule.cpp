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
  dht->humidity().getEvent(&event);

  // Get daylight value
  daylight=0;

  for (int i=0; i<10; i++)
  {
    values[i]=analogRead(PIN_A0);
    daylight=daylight+values[i];
    Serial.println(values[i]);  
    delay(1000);  
  }

  daylight=daylight/10;  

  return true;
}

void OutdoorModule::publishValues() {
  

  sprintf(stringValue, "%f", event.relative_humidity); 
  if(!publishMQTT("/outdoor/humidity/value",  stringValue))
  {
    Serial.println("Error publishing to MQTT server.");
    return;
  }

  sprintf(stringValue, "%f", event.temperature); 
  if(!publishMQTT("/outdoor/temperature/value",  stringValue))
  {
    Serial.println("Error publishing to MQTT server.");
    return;
  }


  sprintf(stringValue, "%d", daylight); 
  if(!publishMQTT(mqttTopic,  stringValue))
  {
    Serial.println("Error publishing to MQTT server.");
    return;
  }

}

