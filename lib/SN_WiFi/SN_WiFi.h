#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiUDP.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 


void SN_WiFi_Init();

bool SN_WiFi_IsConnected();

IPAddress SN_WiFi_getlocalIP();

void SN_WiFi_printlocalIP();

void SN_WiFi_printSSID();

void SN_WiFi_startWebServer();

