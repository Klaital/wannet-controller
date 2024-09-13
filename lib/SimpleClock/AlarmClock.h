//
// Created by Kit on 9/13/2024.
//

#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include "Alarm.h"

#ifndef ALARM_MAX_COUNT
#define ALARM_MAX_COUNT 3
#endif

#ifndef TICK_HANDLER_MAX_COUNT
#define TICK_HANDLER_MAX_COUNT 3
#endif

namespace Wan {

class AlarmClock {
public:
     int tz_offset = -7;
     Alarm alarms[ALARM_MAX_COUNT];
     size_t alarm_count = 0;
     AlarmHandler tick_handlers[TICK_HANDLER_MAX_COUNT];
     size_t ticker_count = 0;

     AlarmClock(): alarms{}, tick_handlers{} {}
     explicit AlarmClock(const int tz_offset): tz_offset(tz_offset), alarms{}, tick_handlers{} {}

     // Configure an alarm.
     void set_alarm(unsigned long day_seconds, AlarmHandler f);
     void add_tick_handler(AlarmHandler h);
     // check if any alarms need to fire now
     void tick();
};

} // Wan

#endif //ALARMCLOCK_H
