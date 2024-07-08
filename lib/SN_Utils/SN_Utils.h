
#include <stdint.h>
#include <cstdint>
#include <string.h>
#include <Arduino.h>

#define STATE_JUST_STARTED 0
#define STATE_WIFI_CONNECTED 1
#define STATE_WIFI_NOT_CONNECTED 2
#define STATE_NTP_SYNC_SUCCESS 3
#define STATE_NTP_SYNC_FAILED 4
#define STATE_READY 5

void SN_Utils_ChangeState(int state);

uint64_t SN_Utils__GetTimestamp();

String SN_Utils__TimestampToStr(uint64_t timestamp);