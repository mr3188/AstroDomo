
#include "adbase.hpp"
#include<map>
#include<string>


adbase::adbase()
{
    readConfigFile();
}

adbase::~adbase()
{
    
}

int adbase::getModuleType()
{
  return moduleType;
}


void adbase::connectWiFi()
{  
  Serial.print("Connecting Wifi...");

  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" done.");

  Serial.println("IP: "+WiFi.localIP().toString());
  
}

void adbase::disconnectWiFi()
{
  Serial.println("Disconnecting from WiFi");
  WiFi.disconnect();
  
}

void adbase::readConfigFile()
{
    Serial.println("Mounting fs");

  delay(3000);
  if(!SPIFFS.begin())
    Serial.println("ERROR mounting fs.");

  File configFile = SPIFFS.open("/config.cfg", "r");
  
  if (!configFile) {
    Serial.println("config file open failed");
  }
  else
  {
      Serial.println("Reading config from File:");

      while(configFile.available())
      {
        String configLine=configFile.readStringUntil('\n');
        configLine.trim();
        configLine.replace(" ", "");
        
        if(configLine.startsWith("module_type"))
        {              
          moduleType=atoi(configLine.substring(configLine.indexOf("=")+1).c_str() );            
          Serial.print("Value of module_type=");
          Serial.println(moduleType);
        }
        else if(configLine.startsWith("mqttClientName"))
        {                    
          configLine.toCharArray(mqttClientName, sizeof(mqttClientName),configLine.indexOf("=")+1 );
          Serial.print("Value of mqttClientName=");
          Serial.println(mqttClientName);
        }
        
        else if(configLine.startsWith("host_name"))
        {                    
          configLine.toCharArray(hostname, sizeof(hostname),configLine.indexOf("=")+1 );
          Serial.print("Value of hostname=");
          Serial.println(hostname);
        }
        else if(configLine.startsWith("ssid"))
        {                    
          configLine.toCharArray(ssid, sizeof(ssid),configLine.indexOf("=")+1 );
          Serial.print("Value of ssid=");
          Serial.println(ssid);
        }
        else if(configLine.startsWith("pass"))
        {                    
          configLine.toCharArray(pass, sizeof(pass),configLine.indexOf("=")+1 );
          Serial.print("Value of pass=");
          Serial.println(pass);
        }        
        else if(configLine.startsWith("mqttServer"))
        {                    
          configLine.toCharArray(mqttServer, sizeof(mqttServer),configLine.indexOf("=")+1 );
          Serial.print("Value of mqttServer=");
          Serial.println(mqttServer);
        }
        else if(configLine.startsWith("mqttTopic"))
        {                    
          configLine.toCharArray(mqttTopic, sizeof(mqttTopic),configLine.indexOf("=")+1 );
          Serial.print("Value of mqttTopic=");
          Serial.println(mqttTopic);
        }
        else if(configLine.startsWith("mqttPort"))
        {              
          mqttPort=atoi(configLine.substring(configLine.indexOf("=")+1).c_str() );            
          Serial.print("Value of mqttPort=");
          Serial.println(mqttPort);
        }
        else if(configLine.startsWith("sleepTime"))
        {                              
          sleepTime=atoi(configLine.substring(configLine.indexOf("=")+1).c_str() );     
          Serial.print("Value of sleepTime=");
          Serial.println(sleepTime);
        }
        
       
      }
      configFile.close();  //Close file
      Serial.println("File Closed");
  }

  SPIFFS.end();
}


bool adbase::connectMQTT()
{
    // Connect to wifi and send a startup message by MQTT

  mqttClient.setClient(wifiClient);
  mqttClient.setServer(mqttServer, mqttPort);
  
  if(!mqttClient.connect(mqttClientName))
  {
    Serial.println("Error connecting to MQTT server.");
    return false;
  }

  return true;
}

bool adbase::publishMQTT(std::string topic, std::string payload)
{
    if(!adbase::wifiClient.connected())
        connectWiFi();
    
    if (!adbase::mqttClient.connected())
        connectMQTT();

    return mqttClient.publish(topic.c_str(), payload.c_str());
}

void adbase::printNetworkStatus()
{
  Serial.println();
  Serial.print("wifi status = ");
  Serial.println(WiFi.status());
  Serial.print("mqtt status = ");
  Serial.println(mqttClient.state());
}

/**
 * Check voltage reading voltage in A0
 * Wiring A0 to voltage imput is needed
 **/
int adbase::chkVoltage()
{
  return 0;
}

 bool adbase::readSensorValues()
 {
   return true;
 }

 void adbase::publishValues()
 {}

 int adbase::getSleepTime()
 {
   return sleepTime;
 }