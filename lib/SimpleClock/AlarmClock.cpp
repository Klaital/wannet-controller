//
// Created by Kit on 9/13/2024.
//

#include "AlarmClock.h"

#include "mbed.h"
#include <mbed_mktime.h>

namespace Wan {

void AlarmClock::set_alarm(const unsigned long day_seconds, const AlarmHandler f) {
    if (this->alarm_count >= ALARM_MAX_COUNT) {
        return;
    }
    this->alarms[this->alarm_count].set(day_seconds, f);
    this->alarm_count++;
}

void AlarmClock::add_tick_handler(const AlarmHandler h) {
    if (this->ticker_count >= TICK_HANDLER_MAX_COUNT) {
        return;
    }
    this->tick_handlers[this->ticker_count] = h;
    this->ticker_count++;
}

void AlarmClock::tick() {
    tm now;
    _rtc_localtime(time(nullptr), &now, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
    // Check each alarm
    for (size_t i=0; i < this->alarm_count; i++) {
        this->alarms[i].tick(now);
    }
    // And run any per-tick handlers
    for (const auto callback : this->tick_handlers) {
        if (callback != nullptr) {
            callback(now);
        }
    }
}
} // Wan
