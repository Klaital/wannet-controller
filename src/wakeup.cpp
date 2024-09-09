//
// Created by Kit on 9/8/2024.
//

#include "mbed.h"

#include <mbed_mktime.h>
#include <ui.h>
#include <WiFi.h>
#include <widgets/label/lv_label.h>

unsigned long wakeup_schedule = (11 * 3600) + (30 * 60) ; // 11:30am
int last_wakeup_day = 0;

void set_wakeup_time(const int hour, const int minute) {
    last_wakeup_day = 0;
    wakeup_schedule = (static_cast<unsigned long>(hour) * 3600) + (static_cast<unsigned long>(minute) * 60);
}

unsigned long day_seconds(const tm &t) {
    return t.tm_sec + (t.tm_min * 60) + (t.tm_hour * 3600);
}
void CheckScheduledEvents() {
    // Get the current time from the RTC module
    tm t;
    _rtc_localtime(time(nullptr), &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT);

    // Update the clock display
    lv_label_set_text_fmt(ui_lblClock, "%02d:%02d", t.tm_hour, t.tm_min);

    // Check if it's time for any events
    if (t.tm_yday >= last_wakeup_day) {
        // already run today
        return;
    }
    if (day_seconds(t) > wakeup_schedule) {
        last_wakeup_day = t.tm_yday;
        Serial.println("Good morning!");
    }


}

// Fetch time from the network using NTP, and adjust it into a unix timestamp.
int timezone = -7;
unsigned int localUdpPort = 2390;
constexpr auto timeServer{ "pool.ntp.org" };
const int NTP_PACKET_SIZE = 48;  // NTP timestamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];  // buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
constexpr unsigned long printInterval{ 1000 };
unsigned long printNow{};

void sendNTPpacket(const char* address);
unsigned long parseNtpPacket();
unsigned long get_ntp_time() {
    Udp.begin(localUdpPort);
    sendNTPpacket(timeServer);
    delay(1000);
    return parseNtpPacket();
}

unsigned long wifi_last_update = 0;
// Get the current time from network, use that to initialize the RTC clock
void rtc_from_ntp() {
    const auto now_ms = millis();
    // Only run the NTP query on startup and once per day to keep the time accurate.
    if (wifi_last_update == 0 || now_ms - wifi_last_update > 86400000ul) { // 86400000 is 1 day in milliseconds

        const auto now = get_ntp_time();

        // Sometimes we won't get a timely response from NTP. Just carry on.
        if (now == 0) {
            return;
        }
        wifi_last_update = now_ms;
        // Update the RTC module
        set_time(now);
    }
}

// send an NTP request to the time server at the given address
void sendNTPpacket(const char* address) {
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;  // LI, Version, Mode
    packetBuffer[1] = 0;           // Stratum, or type of clock
    packetBuffer[2] = 6;           // Polling Interval
    packetBuffer[3] = 0xEC;        // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    Udp.beginPacket(address, 123);  // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

// Parse the response from the NTP server, and adjust it into a Unix timestamp
unsigned long parseNtpPacket() {
    if (!Udp.parsePacket())
        return 0;

    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    const unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    const unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    const unsigned long secsSince1900 = highWord << 16 | lowWord;
    constexpr unsigned long seventyYears = 2208988800UL;
    const unsigned long epoch = secsSince1900 - seventyYears;
    const unsigned long tz_epoch = epoch + (3600 * timezone); //multiply the timezone with 3600 (1 hour)

#if defined(VERBOSE)
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);
    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // print Unix time:
    Serial.println(epoch);

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch % 86400L) / 3600);  // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
        // In the first 10 minutes of each hour, we'll want a leading '0'
        Serial.print('0');
    }
    Serial.print((epoch % 3600) / 60);  // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
        // In the first 10 seconds of each minute, we'll want a leading '0'
        Serial.print('0');
    }
    Serial.println(epoch % 60);  // print the second
#endif

    return tz_epoch;
}