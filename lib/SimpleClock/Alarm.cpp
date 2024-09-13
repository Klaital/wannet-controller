//
// Created by Kit on 9/13/2024.
//

#include "Alarm.h"

namespace Wan {
unsigned long day_seconds(const tm& t) {
    return t.tm_sec + (t.tm_min * 60) + (t.tm_hour * 3600);
}
void Alarm::set(const unsigned long target, const AlarmHandler h) {
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
} // Wan