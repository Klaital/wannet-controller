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
    // how many seconds into the day the alarm should trigger
    unsigned long target = 0;
    // what function to invoke when the alarm triggers
    AlarmHandler handler = nullptr;
    // the last time this alarm triggered.
    int last_wakeup_day = 0;
public:
    Alarm() = default;
    explicit Alarm(const unsigned long target): target(target) {}
    void set(unsigned long target, AlarmHandler h);

    // Check whether the alarm needs to fire
    void tick(const tm& now);
};

} // Wan

#endif //ALARM_H
