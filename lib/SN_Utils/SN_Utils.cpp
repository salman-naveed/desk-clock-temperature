#include <SN_Utils.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <SN_Logger.h>
#include <sys/time.h>
#include <chrono>

extern int CURRENT_STATE;

void SN_Utils_ChangeState(int new_state) {
    CURRENT_STATE = new_state;
}

uint64_t SN_Utils__GetTimestamp() {
    // return (((uint64_t) (GPS_Utils__GetNow())) * 1000) + ((uint64_t) ((esp_timer_get_time() / 1000) % 1000));
    //
    // struct timeval tp;
    // gettimeofday(&tp, NULL);
    // return (uint64_t) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    //
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

String SN_Utils__TimestampToStr(uint64_t timestamp) {
    char data[24] = {0};
    sprintf(data, "%llu", timestamp);

    return String(data);
}