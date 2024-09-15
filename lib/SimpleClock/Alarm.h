// //
// // Created by Kit on 9/13/2024.
// //
//
#ifndef ALARM_H
#define ALARM_H

#include <ctime>

namespace Wan {
// Calculate how many seconds into the day a timestamp is.
unsigned long day_seconds(const tm &t);

typedef void (*AlarmHandler)(const tm &now);

class Alarm {
public:
    // A name for the alarm
    char name[16] = "";
    // how many seconds into the day the alarm should trigger
    unsigned long target = 0;
    // what function to invoke when the alarm triggers
    AlarmHandler handler = nullptr;
    // the last time this alarm triggered.
    int last_wakeup_day = 0;

    Alarm() = default;
    explicit Alarm(const unsigned long target): target(target) {}
    void set(const char* name, unsigned long target, AlarmHandler h);

    // Check whether the alarm needs to fire
    void tick(const tm& now);

    // Add a line to a buffer saying "1:32 left until wakeup time"
    void append_countdown_line(char *buf, tm *now) const;
};

} // Wan

#endif //ALARM_H
