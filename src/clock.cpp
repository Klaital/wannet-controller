//
// Created by Kit on 9/13/2024.
//
#include <WiFi.h>
#include "mbed.h"
#include <mbed_mktime.h>

extern int TimezoneOffset;
unsigned int localUdpPort = 2390;
constexpr auto timeServer{ "pool.ntp.org" };
const int NTP_PACKET_SIZE = 48;  // NTP timestamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];  // buffer to hold incoming and outgoing packets
WiFiUDP Udp;

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
    const unsigned long tz_epoch = epoch + (3600 * TimezoneOffset); //multiply the timezone with 3600 (1 hour)

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