#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WifiUDP.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 

#define  MAX_REST_MESSAGE_SIZE 1450


void SN_WiFi_Init();

bool SN_WiFi_IsConnected();

IPAddress SN_WiFi_getlocalIP();

void SN_WiFi_printlocalIP();

void SN_WiFi_printSSID();

void SN_WiFi_startWebServer();

void SN_WiFi_handleClient();

