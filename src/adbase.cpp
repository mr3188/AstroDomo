
#include "adbase.hpp"
#include<map>
#include<string>


adbase::adbase()
{
    
}

adbase::~adbase()
{
    
}

int adbase::moduleType()
{

  return 0;
}


/**
 * Do all loop work for this node

void adbase::loop()
{

  // all node types should check voltage
  // if input voltage is under a given value an mqtt message must be published


  // Read config file so we can identify module class
  //

}
**/

void adbase::connectWiFi()
{  
  Serial.print("Connecting Wifi...");

  WiFi.mode(WIFI_STA);
  WiFi.hostname("outdoormodule");
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
        
        if(configLine.startsWith("ssid"))
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

  adbase::connectWiFi();

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

}