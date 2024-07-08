#include <SN_WiFi.h>
#include <SN_Utils.h>
#include <SN_LCD_I2C.h> 
#include <SN_Logger.h>
#include <SN_Time.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
// #include <HttpClient.h>
#include <ESP8266HTTPClient.h>

extern int CURRENT_STATE;

ESP8266WebServer SN_WebServer(80);

const char* ssid = "Wayne_ENT";     //"Salman_C25"; //"Wayne_ENT";   // insert your own ssid 
const char* password = "12345678";   


void SN_WiFi_Init() {
    WiFi.begin(ssid, password);
    
    SN_Logger_Log(true, "SN_WiFi", "Connecting to WiFi");
    SN_LCD_I2C_print("Connecting to WiFi");
    // SN_LCD_I2C_autoscroll();

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
        // SN_LCD_I2C_autoscroll();

        if (MDNS.begin("SN_esp8266")) {              // Start the mDNS responder for esp8266.local
            Serial.println("mDNS responder started");
        } else {
            Serial.println("Error setting up MDNS responder!");
        }
    }

    delay(500);
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

void SN_WiFi_handleClient() {
    SN_WebServer.handleClient();
}

void SendJsonMessage(String json, int httpCode) {
    SN_WebServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    SN_WebServer.send(httpCode, "text/json", json);
}

void SendHTMLMessage(String html, int httpCode) {
    SN_WebServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    SN_WebServer.send(httpCode, "text/html", html);
}

void WiFiScan_Handler() {
    int n = WiFi.scanNetworks();

    DynamicJsonBuffer jsonBuffer(MAX_REST_MESSAGE_SIZE);

    JsonObject &root = jsonBuffer.createObject();

    root[F("number_of_networks")] = n;

    JsonArray &networks = root.createNestedArray("networks");

    for (uint8_t i = 0; i < n; i++) {
        String name = WiFi.SSID(i);
        networks.add(name);
    }

    String json = String();
    json = "";
    root.printTo(json);

    SendJsonMessage(json, HTTP_CODE_OK);
}


// void WiFiCheckStatus_Handler() {
//     String msg = String("POST request to '/get_wifi_check_status'");
//     Serial.println(msg);
//     Serial.flush();

//     String json = String();
//     json = "{";

//     // #define WIFI_CHECK_STATUS_NEED_TO_CHECK 0
//     // #define WIFI_CHECK_STATUS_IN_PROGRESS 1
//     // #define WIFI_CHECK_STATUS_SUCCESSFULLY 2
//     // #define WIFI_CHECK_STATUS_FAILED 3
//     json += String("\"check_wifi_status\":") + check_wifi_status;
//     json += "}";

//     msg = String("POST response: ") + json;
//     Serial.println(msg);
//     Serial.flush();

//     SendJsonMessage(json, HTTP_CODE_OK);
// }

void GetIdentificationParameters_Handler() {
    DynamicJsonBuffer jsonBuffer(MAX_REST_MESSAGE_SIZE);
    JsonObject &root = jsonBuffer.createObject();

    root[F("VendorName")] = "SN Technologies Inc.";
    root[F("ProductName")] = "SN-ESP8266-CLK";
    root[F("ArticleNumber")] = "20-09-2340";
    root[F("UniqueID")] = WiFi.macAddress();
    root[F("SerialNumber")] = "23098576";
    root[F("ProductionCode")] = "L24";
    root[F("HardwareRevision")] = "2";

    root[F("Device_Type")] = 2; // With or Without ethernet support
    root[F("API_Version")] = 3;

    String json = String();
    json = "";
    root.printTo(json);

    SendJsonMessage(json, HTTP_CODE_OK);
}

void SendJsonStatus(int httpCode) {
    String status = httpCode == HTTP_CODE_OK ? "OK" : "Error";

    String json = String();
    json = "{";
    json += String("\"status\":\"") + status + "\"";
    json += "}";

    SendJsonMessage(json, httpCode);
}

void handleRoot() {
    SN_WebServer.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
    SN_WebServer.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void TimezoneChange_Handler() {
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

void GetTime_Handler() {
    String message = "Time: ";
    message += SN_Utils__TimestampToStr(SN_Utils__GetTimestamp());

    SN_WebServer.send(200, "text/plain", message);
}


void SN_WiFi_startWebServer() {

    SN_WebServer.on("/", HTTP_GET, handleRoot);

    SN_WebServer.on("/status", HTTP_GET, DeviceStatus_Handler);

    SN_WebServer.on("/timezone_change", HTTP_POST, TimezoneChange_Handler);

    SN_WebServer.on("/identification", HTTP_GET, GetIdentificationParameters_Handler);

    SN_WebServer.on("/scan", HTTP_GET, WiFiScan_Handler);

    SN_WebServer.on("/get_time", HTTP_GET, GetTime_Handler);

    SN_WebServer.onNotFound(handleNotFound);

    // Start the server
    SN_WebServer.begin();

    SN_Logger_Log(true, "SN_WiFi", "Server started");

    // SN_LCD_I2C_print("Server started");
}