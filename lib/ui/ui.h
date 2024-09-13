// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: UI

#ifndef _UI_UI_H
#define _UI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_TabView1;
extern lv_obj_t * ui_TabClock;
extern lv_obj_t * ui_Clock;
extern lv_obj_t * ui_WakeupCountdown;
extern lv_obj_t * ui_TabTV;
extern lv_obj_t * ui_PlaylstSelection;
void ui_event_ButtonPlaylistUp(lv_event_t * e);
extern lv_obj_t * ui_ButtonPlaylistUp;
extern lv_obj_t * ui_Label9;
void ui_event_ButtonPlaylistDown(lv_event_t * e);
extern lv_obj_t * ui_ButtonPlaylistDown;
extern lv_obj_t * ui_Label10;
void ui_event_ButtonPlaylistSelect(lv_event_t * e);
extern lv_obj_t * ui_ButtonPlaylistSelect;
extern lv_obj_t * ui_Label11;
extern lv_obj_t * ui_TabLights;
void ui_event_LightsDimmer(lv_event_t * e);
extern lv_obj_t * ui_LightsDimmer;
extern lv_obj_t * ui_LabelDimmerPct;
extern lv_obj_t * ui_LabelWakeupTime;
extern lv_obj_t * ui_KeyboardWakeup;
extern lv_obj_t * ui_WakeupTimeSpinbox;
void ui_event_SetWakeup(lv_event_t * e);
extern lv_obj_t * ui_SetWakeup;
extern lv_obj_t * ui_LabelSubmitWakeup;
void ui_event_LightSwitch(lv_event_t * e);
extern lv_obj_t * ui_LightSwitch;
extern lv_obj_t * ui_LabelLights;
extern lv_obj_t * ui____initial_actions0;








void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
