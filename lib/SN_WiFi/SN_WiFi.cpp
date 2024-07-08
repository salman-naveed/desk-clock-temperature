#include <SN_WiFi.h>
#include <SN_Utils.h>
#include <SN_LCD_I2C.h> 
#include <SN_Logger.h>
#include <SN_Time.h>

extern int CURRENT_STATE;

ESP8266WebServer SN_WebServer(80);

const char* ssid = "Wayne_ENT";     //"Salman_C25"; //"Wayne_ENT";   // insert your own ssid 
const char* password = "12345678";   


void SN_WiFi_Init() {
    WiFi.begin(ssid, password);
    
    SN_Logger_Log(true, "SN_WiFi", "Connecting to WiFi");
    SN_LCD_I2C_print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) 
        {
            delay(500);
            SN_Logger_Log(true, "SN_WiFi", ".");
        }
    
    if (WiFi.isConnected()) {
        SN_Utils_ChangeState(STATE_WIFI_CONNECTED);
        SN_Logger_Log(true, "SN_WiFi", "Connected to WiFi");
    
        SN_WiFi_printlocalIP();
        SN_WiFi_printSSID();

        SN_LCD_I2C_print("Connected to WiFi");

        if (MDNS.begin("SN_esp8266")) {              // Start the mDNS responder for esp8266.local
            Serial.println("mDNS responder started");
        } else {
            Serial.println("Error setting up MDNS responder!");
        }
    }
}

bool SN_WiFi_IsConnected() {
    return WiFi.isConnected();
}

IPAddress SN_WiFi_getlocalIP() {
    return WiFi.localIP();
}

void SN_WiFi_printlocalIP() {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void SN_WiFi_printSSID() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
}

void handleRoot() {
    SN_WebServer.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
    SN_WebServer.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleTimezoneChange() {
    String message = "Timezone changed to: ";
    message += SN_WebServer.arg("timezone");
    SN_WebServer.send(200, "text/plain", message);

    int timezone = SN_WebServer.arg("timezone").toInt();

    SN_Time_changeTimeZone(timezone);
}

void DeviceStatus_Handler() {
    String message = "Device Status: ";
    message += CURRENT_STATE;

    SN_WebServer.send(200, "text/plain", message);
}



void SN_WiFi_startWebServer() {

    SN_WebServer.on("/", HTTP_GET, handleRoot);

    SN_WebServer.on("/status", HTTP_GET, DeviceStatus_Handler);

    SN_WebServer.on("/inline", HTTP_POST, handleTimezoneChange);

    SN_WebServer.onNotFound(handleNotFound);

    // Start the server
    SN_WebServer.begin();

    SN_Logger_Log(true, "SN_WiFi", "Server started");

    // SN_LCD_I2C_print("Server started");
}