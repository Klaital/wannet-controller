//
// Created by Kit on 9/13/2024.
//

#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <core/lv_obj.h>

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

     // GUI widgets to update on tick
     lv_obj_t *clock_widget = nullptr;
     lv_obj_t *alarm_countdown_widget = nullptr;

     AlarmClock(): alarms{}, tick_handlers{} {}
     explicit AlarmClock(const int tz_offset): tz_offset(tz_offset), alarms{}, tick_handlers{} {}

     // Configure an alarm.
     void set_alarm(const char* name, unsigned long day_seconds, AlarmHandler f);
     void add_tick_handler(AlarmHandler h);

     // Add UI widgets to be updated
     void configure_lvgl_digital_clock(lv_obj_t *clock);
     void configure_lvgl_countdowns(lv_obj_t *lbl);

     // check if any alarms need to fire now
     void tick();
};

} // Wan

#endif //ALARMCLOCK_H
