//
// Created by Kit on 9/13/2024.
//

#include "Alarm.h"

#include <cstdio>
#include <cstring>

namespace Wan {
unsigned long day_seconds(const tm& t) {
    return t.tm_sec + (t.tm_min * 60) + (t.tm_hour * 3600);
}
void Alarm::set(const char* name, const unsigned long target, const AlarmHandler h) {
    strcpy(this->name, name);
    this->target = target;
    this->handler = h;
    this->last_wakeup_day = 0;
}

void Alarm::tick(const tm& now) {

    // Check whether this alarm has already fired today
    if (now.tm_yday == this->last_wakeup_day) {
        return;
    }

    // Check whether the current time is after the alarm time
    if (day_seconds(now) > this->target) {
        last_wakeup_day = now.tm_yday;
        if (this->handler != nullptr) {
            this->handler(now);
        }
    }
}

void Alarm::append_countdown_line(char *buf, tm *now) const {
    char tmp[40] = "";
    const auto day_time = day_seconds(*now);
    // If the alarm has already fired today, write that
    if (day_time > this->target) {
        strcat(buf, this->name);
        strcat(buf, " already fired today\n");
        return;
    }
    // TODO: calculate hours and minutes left until the trigger
    sprintf(tmp, "%lu minutes until %s time\n", (day_time - this->target)/60, this->name);
    strcat(buf, tmp);
}
} // Wan