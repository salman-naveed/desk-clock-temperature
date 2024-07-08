#include <WifiUDP.h>
#include <String.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

#define NTP_TIME_SYNC_INTERVAL 1200  // seconds


time_t SN_Time_getNtpTime();

void SN_Time_sendNTPpacket(IPAddress &address);

void SN_Time_Init();

int SN_Time_getTimeStatus();

void SN_Time_setTimezone(int timezone);

void SN_Time_changeTimeZone(int newTimeZone);