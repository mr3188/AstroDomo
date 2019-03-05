
#ifndef ADBASE_H
#define ADBASE_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FS.h>   //Include File System Headers
#include <map>
#include <string>

class 
adbase
{
private:

    int moduleType;
    char hostname[128];
    //SSID of your network
    char ssid[128]; //SSID of your Wi-Fi router
    char pass[64] ; //Password of your Wi-Fi router
    const char* filename ;
    
    char mqttClientName[128];
    char mqttServer[128] ;
    uint mqttPort=1883;

    std::map<std::string, std::string> config;
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    

    // Sleep for 5 minutes
    int sleepTime=300;

    int chkVoltage();

protected:
    char mqttTopic[128];       
    void disconnectWiFi();
    void connectWiFi();
    void readConfigFile();
    bool connectMQTT();
    bool publishMQTT(std::string topic, std::string payload);
    void printNetworkStatus();

public:
    adbase();
    ~adbase();
    
    int getModuleType();
    int getSleepTime();
    virtual bool readSensorValues();
    virtual void publishValues();
};

#endif
