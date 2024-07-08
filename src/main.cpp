// File: main.cpp

#include <SN_DHT.h>
#include <SN_LCD_I2C.h>
#include <SN_Time.h>
#include <SN_WiFi.h>
#include <SN_Logger.h>
#include <SN_Utils.h>

int CURRENT_STATE = STATE_JUST_STARTED;

time_t prevDisplay = 0; // when the digital clock was displayed


void setup() {
  SN_UART_Init();

  SN_DHT_Init();
  
  SN_LCD_I2C_Init();

  SN_WiFi_Init();

  SN_WiFi_startWebServer();
  
  SN_Time_Init();

}

void loop() {

  switch (CURRENT_STATE) {
    case STATE_READY:
      if (timeStatus() != timeNotSet) {
        if (now() != prevDisplay) { //update the display only if time has changed
          prevDisplay = now();
          SN_LCD_I2C_clockDisplay();
        }
      }  
      break;

    default:
      break;
  }
}

