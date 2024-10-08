// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: UI

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_TabView1 = lv_tabview_create(ui_Screen1, LV_DIR_BOTTOM, 30);
    lv_obj_set_width(ui_TabView1, 800);
    lv_obj_set_height(ui_TabView1, 480);
    lv_obj_set_align(ui_TabView1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_TabView1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags


    ui_TabClock = lv_tabview_add_tab(ui_TabView1, "Clock");

    ui_Clock = lv_label_create(ui_TabClock);
    lv_obj_set_width(ui_Clock, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Clock, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Clock, -7);
    lv_obj_set_y(ui_Clock, -150);
    lv_obj_set_align(ui_Clock, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Clock, "HH:mm");
    lv_obj_set_style_text_font(ui_Clock, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WakeupCountdown = lv_label_create(ui_TabClock);
    lv_obj_set_width(ui_WakeupCountdown, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_WakeupCountdown, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_WakeupCountdown, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WakeupCountdown, "21 minutes until wakeup");

    ui_TabTV = lv_tabview_add_tab(ui_TabView1, "TV");

    ui_PlaylstSelection = lv_roller_create(ui_TabTV);
    lv_roller_set_options(ui_PlaylstSelection, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_PlaylstSelection, 250);
    lv_obj_set_height(ui_PlaylstSelection, 208);
    lv_obj_set_x(ui_PlaylstSelection, 0);
    lv_obj_set_y(ui_PlaylstSelection, 93);
    lv_obj_set_align(ui_PlaylstSelection, LV_ALIGN_CENTER);

    ui_ButtonPlaylistUp = lv_btn_create(ui_TabTV);
    lv_obj_set_width(ui_ButtonPlaylistUp, 100);
    lv_obj_set_height(ui_ButtonPlaylistUp, 60);
    lv_obj_set_x(ui_ButtonPlaylistUp, -190);
    lv_obj_set_y(ui_ButtonPlaylistUp, 20);
    lv_obj_set_align(ui_ButtonPlaylistUp, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonPlaylistUp, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonPlaylistUp, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PlaylistUpLabel = lv_label_create(ui_ButtonPlaylistUp);
    lv_obj_set_width(ui_PlaylistUpLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PlaylistUpLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PlaylistUpLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PlaylistUpLabel, "Up");

    ui_ButtonPlaylistDown = lv_btn_create(ui_TabTV);
    lv_obj_set_width(ui_ButtonPlaylistDown, 100);
    lv_obj_set_height(ui_ButtonPlaylistDown, 60);
    lv_obj_set_x(ui_ButtonPlaylistDown, -190);
    lv_obj_set_y(ui_ButtonPlaylistDown, 175);
    lv_obj_set_align(ui_ButtonPlaylistDown, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonPlaylistDown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonPlaylistDown, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PlaylistDownLabel = lv_label_create(ui_ButtonPlaylistDown);
    lv_obj_set_width(ui_PlaylistDownLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PlaylistDownLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PlaylistDownLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PlaylistDownLabel, "Down");

    ui_ButtonPlaylistSelect = lv_btn_create(ui_TabTV);
    lv_obj_set_width(ui_ButtonPlaylistSelect, 100);
    lv_obj_set_height(ui_ButtonPlaylistSelect, 60);
    lv_obj_set_x(ui_ButtonPlaylistSelect, -190);
    lv_obj_set_y(ui_ButtonPlaylistSelect, 97);
    lv_obj_set_align(ui_ButtonPlaylistSelect, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonPlaylistSelect, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonPlaylistSelect, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PlaylistSelectLabel = lv_label_create(ui_ButtonPlaylistSelect);
    lv_obj_set_width(ui_PlaylistSelectLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PlaylistSelectLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PlaylistSelectLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PlaylistSelectLabel, "Select");

    ui_SkipAhead = lv_btn_create(ui_TabTV);
    lv_obj_set_width(ui_SkipAhead, 100);
    lv_obj_set_height(ui_SkipAhead, 60);
    lv_obj_set_x(ui_SkipAhead, 257);
    lv_obj_set_y(ui_SkipAhead, 29);
    lv_obj_set_align(ui_SkipAhead, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SkipAhead, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SkipAhead, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_SkipAheadLabel = lv_label_create(ui_SkipAhead);
    lv_obj_set_width(ui_SkipAheadLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SkipAheadLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_SkipAheadLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SkipAheadLabel, "Skip");

    ui_PlayPauseButton = lv_btn_create(ui_TabTV);
    lv_obj_set_width(ui_PlayPauseButton, 100);
    lv_obj_set_height(ui_PlayPauseButton, 60);
    lv_obj_set_x(ui_PlayPauseButton, 262);
    lv_obj_set_y(ui_PlayPauseButton, 130);
    lv_obj_set_align(ui_PlayPauseButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PlayPauseButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_PlayPauseButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PlayPauseLabel = lv_label_create(ui_PlayPauseButton);
    lv_obj_set_width(ui_PlayPauseLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PlayPauseLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PlayPauseLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PlayPauseLabel, "Pause");

    ui_PlaylistIdDisplay = lv_label_create(ui_TabTV);
    lv_obj_set_width(ui_PlaylistIdDisplay, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PlaylistIdDisplay, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_PlaylistIdDisplay, -23);
    lv_obj_set_y(ui_PlaylistIdDisplay, -42);
    lv_obj_set_align(ui_PlaylistIdDisplay, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PlaylistIdDisplay, "");

    ui_TabLights = lv_tabview_add_tab(ui_TabView1, "Lights");

    ui_LightsDimmer = lv_slider_create(ui_TabLights);
    lv_slider_set_value(ui_LightsDimmer, 0, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_LightsDimmer) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_LightsDimmer, 0,
                                                                                                 LV_ANIM_OFF);
    lv_obj_set_width(ui_LightsDimmer, 541);
    lv_obj_set_height(ui_LightsDimmer, 64);
    lv_obj_set_x(ui_LightsDimmer, 4);
    lv_obj_set_y(ui_LightsDimmer, -86);
    lv_obj_set_align(ui_LightsDimmer, LV_ALIGN_CENTER);


    ui_LabelDimmerPct = lv_label_create(ui_TabLights);
    lv_obj_set_width(ui_LabelDimmerPct, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelDimmerPct, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelDimmerPct, 207);
    lv_obj_set_y(ui_LabelDimmerPct, -157);
    lv_obj_set_align(ui_LabelDimmerPct, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelDimmerPct, "12%");

    ui_LabelWakeupTime = lv_label_create(ui_TabLights);
    lv_obj_set_width(ui_LabelWakeupTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelWakeupTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelWakeupTime, -293);
    lv_obj_set_y(ui_LabelWakeupTime, 60);
    lv_obj_set_align(ui_LabelWakeupTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelWakeupTime, "Wakeup Time:");

    ui_KeyboardWakeup = lv_keyboard_create(ui_TabLights);
    lv_keyboard_set_mode(ui_KeyboardWakeup, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_set_width(ui_KeyboardWakeup, 300);
    lv_obj_set_height(ui_KeyboardWakeup, 120);
    lv_obj_set_x(ui_KeyboardWakeup, -205);
    lv_obj_set_y(ui_KeyboardWakeup, 134);
    lv_obj_set_align(ui_KeyboardWakeup, LV_ALIGN_CENTER);

    ui_WakeupTimeSpinbox = lv_spinbox_create(ui_TabLights);
    lv_obj_set_width(ui_WakeupTimeSpinbox, 63);
    lv_obj_set_height(ui_WakeupTimeSpinbox, 30);
    lv_obj_set_x(ui_WakeupTimeSpinbox, -199);
    lv_obj_set_y(ui_WakeupTimeSpinbox, 57);
    lv_obj_set_align(ui_WakeupTimeSpinbox, LV_ALIGN_CENTER);
    lv_spinbox_set_digit_format(ui_WakeupTimeSpinbox, 4, 2);
    lv_spinbox_set_range(ui_WakeupTimeSpinbox, 0, 9999);
    lv_spinbox_set_cursor_pos(ui_WakeupTimeSpinbox, 1 - 1);
    lv_spinbox_set_value(ui_WakeupTimeSpinbox, 530);

    ui_SetWakeup = lv_btn_create(ui_TabLights);
    lv_obj_set_width(ui_SetWakeup, 83);
    lv_obj_set_height(ui_SetWakeup, 29);
    lv_obj_set_x(ui_SetWakeup, -115);
    lv_obj_set_y(ui_SetWakeup, 57);
    lv_obj_set_align(ui_SetWakeup, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SetWakeup, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetWakeup, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelSubmitWakeup = lv_label_create(ui_SetWakeup);
    lv_obj_set_width(ui_LabelSubmitWakeup, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelSubmitWakeup, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSubmitWakeup, -1);
    lv_obj_set_y(ui_LabelSubmitWakeup, 0);
    lv_obj_set_align(ui_LabelSubmitWakeup, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSubmitWakeup, "Set");

    ui_LightSwitch = lv_switch_create(ui_TabLights);
    lv_obj_set_width(ui_LightSwitch, 142);
    lv_obj_set_height(ui_LightSwitch, 66);
    lv_obj_set_x(ui_LightSwitch, 0);
    lv_obj_set_y(ui_LightSwitch, -168);
    lv_obj_set_align(ui_LightSwitch, LV_ALIGN_CENTER);


    ui_LabelLights = lv_label_create(ui_TabLights);
    lv_obj_set_width(ui_LabelLights, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLights, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLights, -221);
    lv_obj_set_y(ui_LabelLights, -153);
    lv_obj_set_align(ui_LabelLights, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLights, "Lights Power");

    lv_obj_add_event_cb(ui_ButtonPlaylistUp, ui_event_ButtonPlaylistUp, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ButtonPlaylistDown, ui_event_ButtonPlaylistDown, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ButtonPlaylistSelect, ui_event_ButtonPlaylistSelect, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SkipAhead, ui_event_SkipAhead, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PlayPauseButton, ui_event_PlayPauseButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_LightsDimmer, ui_event_LightsDimmer, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SetWakeup, ui_event_SetWakeup, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_LightSwitch, ui_event_LightSwitch, LV_EVENT_ALL, NULL);

}
