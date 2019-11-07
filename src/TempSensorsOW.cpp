#include "TempSensorsOW.hpp"

/**
 * Print human readeable OneWire address to Serial line
 * 
 **/

void  TempSensorsOW::printAddress(owSensor sensor)
{
  for( int i = 0; i < 8; i++) {
      Serial.print("0x");
      if (sensor.address.values[i] < 16) {
        Serial.print('0');
      }
      // print each byte in the address array in hex format
      Serial.print(sensor.address.values[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
      else
        Serial.println("");
    }
}


/**
 * Search OneWire network to find connected devices
 * List of devices are stored in an array
 */

void TempSensorsOW::getDeviceAddress() {
    
  Serial.println("Search for devices...\n\r");
  
  devNumber=0;
  while(ds->search(sensors[devNumber].address.values)) {
   
    printAddress(sensors[devNumber]);

    // a check to make sure that what we read is correct.

     byte crc=OneWire::crc8(sensors[devNumber].address.values, 7);

    if ( crc != sensors[devNumber].address.values[7]) {
        Serial.print("CRC is not valid!\n");
        //return;
    } 

    
    devNumber++;
  }
  ds->reset_search();
}

TempSensorsOW::TempSensorsOW()
{
  ds=new OneWire(DS18S20_Pin);  // temperature sensors
  getDeviceAddress();
  
}

TempSensorsOW::~TempSensorsOW()
{
  delete(ds);
}



float  TempSensorsOW::getTemp(owSensor sensor){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];

  ds->reset();
  ds->select(sensor.address.values);
  ds->write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000);
  ds->reset();
  ds->select(sensor.address.values);    
  ds->write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds->read();
  }

  ds->reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float temp = tempRead / 16;

  sensor.temp=temp;

  return  temp;
}



bool TempSensorsOW::readSensorValues(void) {

  bool doneOk=true;

  for(int i=0 ; i<devNumber; i++)
  {
    sensors[i].temp = getTemp(sensors[i]);   
  }
  delay(1);  

  return doneOk; 
};

void TempSensorsOW::publishValues() {

  for(int i=0 ; i<devNumber; i++)
  {    

    sprintf(stringValue, "{\"sensor_name\": \"%02x%02x%02x%02x%02x%02x%02x%02x\", \"temp\" :  %3f }", 
        sensors[i].address.values[0], 
        sensors[i].address.values[1],
        sensors[i].address.values[2], 
        sensors[i].address.values[3],
        sensors[i].address.values[4], 
        sensors[i].address.values[5], 
        sensors[i].address.values[6],
        sensors[i].address.values[7],  
        sensors[i].temp);
    
    Serial.println(stringValue);
   
    if(!publishMQTT(mqttTopic,  stringValue))
      Serial.println("Error publishing to MQTT server.");
  }
}
  

