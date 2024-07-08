#include <SN_DHT.h>
#include <SN_Logger.h>

DHT dht(DHTPIN, DHTTYPE);   // Configure DHT library

void SN_DHT_Init() {
    dht.begin();
    SN_Logger_Log(true, "SN_DHT", "DHT sensor initialized");
}

float SN_DHT_getTemperature() {
    SN_Logger_Log(true, "SN_DHT", "Temperature: %f", dht.readTemperature());
    return dht.readTemperature();
}

float SN_DHT_getHumidity() {
    SN_Logger_Log(true, "SN_DHT", "Humidity: %f", dht.readHumidity());
    return dht.readHumidity();
}