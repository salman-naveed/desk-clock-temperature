#include "DHT.h"      // Include DHT library


#define DHTPIN 1            // DHT22 data pin is connected to ESP8266 pin GPIO1 (TX)
#define DHTTYPE DHT22       // DHT22 sensor is used

void SN_DHT_Init();

float SN_DHT_getTemperature();

float SN_DHT_getHumidity();
