#include <SN_Time.h>
#include <SN_WiFi.h>
#include <SN_Logger.h>
#include <SN_LCD_I2C.h>
#include <SN_Utils.h>

// NTP Servers:
static const char ntpServerName[] = "ca.pool.ntp.org"; //us.pool.ntp.org
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

// const int timeZone = 2;   // Central European Time GMT+2
int timeZone = 2;   // 1: Central European Summer Time, 2: Central European Time (GMT+2)


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t SN_Time_getNtpTime()
{ 
  SN_Logger_Log(true, "SN_Time", "Getting NTP time");

  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets

  Serial.println("Sending NTP Request");
  
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  
  SN_Time_sendNTPpacket(ntpServerIP);
  
  uint32_t beginWait = millis();
  
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Received NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];

      SN_Utils_ChangeState(STATE_READY);
      
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }

  //1500 secs timed out, return 0
  //Needs to return 0 so setTime does not proceed to 
  //sync with unknown value, see now() in time.cpp
  Serial.println("No NTP Response :-(");

  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void SN_Time_sendNTPpacket(IPAddress &address)
{
  SN_Logger_Log(true, "SN_Time", "Sending NTP packet");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void SN_Time_setTimezone(int timezone) {
  // Set timezone
  TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
  TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Time
  Timezone CE(CEST, CET);
}


void SN_Time_Init() {
    Udp.begin(localPort);
    setSyncProvider(SN_Time_getNtpTime);
    setSyncInterval(NTP_TIME_SYNC_INTERVAL);

    SN_Time_setTimezone(timeZone);

    SN_Logger_Log(true, "SN_Time", "Time initialized");

    SN_LCD_I2C_print("Time init");
}

int SN_Time_getTimeStatus() {
    return timeStatus();
}  

void SN_Time_changeTimeZone(int newTimeZone) {
    timeZone = newTimeZone;
    SN_Time_setTimezone(timeZone);

    SN_Logger_Log(true, "SN_Time", "Timezone changed to %d", timeZone);
}