
#include <Arduino.h>

void SN_UART_Init();

void SN_Logger_Log(bool serial_verbose, String point, const char *format, ...);

void SN_Logger_udpLog(const char *format, ...);