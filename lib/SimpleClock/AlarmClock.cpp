//
// Created by Kit on 9/13/2024.
//

#include "AlarmClock.h"

#include "ui.h"
#include "mbed.h"
#include <mbed_mktime.h>

namespace Wan {

void AlarmClock::set_alarm(const char* name, const unsigned long day_seconds, const AlarmHandler f) {
    // find any existing alarm with the same name
    int alarm_idx = -1;
    for (size_t i=0; i < this->alarm_count; i++) {
        if (strcmp(name, this->alarms[i].name) == 0) {
            alarm_idx = static_cast<int>(i);
            break;
        }
    }

    int add_new = 1;
    if (alarm_idx >= 0) {
        add_new = 0;
    } else {
        alarm_idx = static_cast<int>(this->alarm_count);
    }
    if (alarm_idx  >= ALARM_MAX_COUNT) {
        return;
    }
    this->alarms[alarm_idx].set(name, day_seconds, f);
    this->alarm_count += add_new;
    Serial.print("Adding alarm #");
    Serial.print(this->alarm_count);
    Serial.print(": ");
    Serial.println(name);

}

void AlarmClock::add_tick_handler(const AlarmHandler h) {
    if (this->ticker_count >= TICK_HANDLER_MAX_COUNT) {
        return;
    }
    this->tick_handlers[this->ticker_count] = h;
    this->ticker_count++;
}

void AlarmClock::configure_lvgl_digital_clock(lv_obj_t *clock) {
    this->clock_widget = clock;
}

void AlarmClock::configure_lvgl_countdowns(lv_obj_t *lbl) {
    this->alarm_countdown_widget = lbl;
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

    // Update the GUI widgets, if provided
    if (this->clock_widget != nullptr) {
        lv_label_set_text_fmt(this->clock_widget, "%02d-%02d\n%02d:%02d:%02d", now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
    }
    if (this->alarm_countdown_widget != nullptr) {
        char buf[40 * ALARM_MAX_COUNT] = "";
        for (size_t i=0; i<this->alarm_count; i++) {
            // Serial.print("Getting countdown for ");
            // Serial.print(this->alarms[i].name);
            this->alarms[i].append_countdown_line(buf, &now);
            // Serial.print(": ");
            // Serial.println(buf);
        }
        lv_label_set_text(this->alarm_countdown_widget, buf);
    }
}
} // Wan
