#include "TempSensorsOW.hpp"

/**
 * Print human readeable OneWire address to Serial line
 * 
 **/

void  TempSensorsOW::printAddress(addr deviceAddr)
{
  for( int i = 0; i < 8; i++) {
      Serial.print("0x");
      if (deviceAddr.values[i] < 16) {
        Serial.print('0');
      }
      // print each byte in the address array in hex format
      Serial.print(deviceAddr.values[i], HEX);
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
  while(ds->search(address[devNumber].values)) {
   
    printAddress(address[devNumber]);

    // a check to make sure that what we read is correct.

     byte crc=OneWire::crc8(address[devNumber].values, 7);

    if ( crc != address[devNumber].values[7]) {
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

}

/**
void setup(void) {
 
 
  getDeviceAddress();

  for(int i=0 ; i<devNumber; i++)
  {   
    printAddress(address[i]);
    sprintf(mqttPayload, "%02X%02X%02X%02X%02X%02X%02X%02X", address[i].values[0],address[i].values[1],address[i].values[2], 
        address[i].values[3], address[i].values[4], address[i].values[5], address[i].values[6], address[i].values[7]);
    
    publish("/caldera/temp/startup",  mqttPayload);
    Serial.println(mqttPayload);
  }
}

float  TempSensorsOW::getTemp(addr address){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];

  ds.reset();
  ds.select(address.values);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000);
  ds.reset();
  ds.select(address.values);    
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float temp = tempRead / 16;

  return  temp;

 
}

*/

bool TempSensorsOW::readSensorValues(void) {

  bool doneOk=true;

  for(int i=0 ; i<devNumber; i++)
  {
    float temp = getTemp(address[i]);
    int iTemp = (int) temp;
   
    sprintf(stringValue, "{\"sensor_name\": \"%02x%02x%02x%02x%02x%02x%02x%02x\", \"temp\" :  %3d }", address[i].values[0], address[i].values[1],
      address[i].values[2], address[i].values[3], address[i].values[4], address[i].values[5], address[i].values[6], address[i].values[7],  iTemp);
    
    Serial.println(stringValue);
   
    doneOk= doneOk || publishMQTT(mqttTopic,  stringValue);    
  }
  delay(1);  

  return doneOk; 
};


