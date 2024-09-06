// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: TabbedHomeUI

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_TabView1;
lv_obj_t * ui_TabLights;
lv_obj_t * ui_LightsHeader;
lv_obj_t * ui_LightsOnLabel;
void ui_event_LightsOnSwitch(lv_event_t * e);
lv_obj_t * ui_LightsOnSwitch;
void ui_event_LightsDimmerSlider(lv_event_t * e);
lv_obj_t * ui_LightsDimmerSlider;
lv_obj_t * ui_WakeupTimeRoller;
lv_obj_t * ui_DimmerValueLabel;
void ui_event_WakeupOverrideButton(lv_event_t * e);
lv_obj_t * ui_WakeupOverrideButton;
lv_obj_t * ui_lblWakeupSubmit;
lv_obj_t * ui_TabTV;
lv_obj_t * ui_TVControlHeading;
void ui_event_TvPowerSwitch(lv_event_t * e);
lv_obj_t * ui_TvPowerSwitch;
lv_obj_t * ui_PlaylistSelectRoller;
void ui_event_TvVolumeSlider(lv_event_t * e);
lv_obj_t * ui_TvVolumeSlider;
void ui_event_PlaylistUp(lv_event_t * e);
lv_obj_t * ui_PlaylistUp;
lv_obj_t * ui_lblPlaylistUp;
void ui_event_SelectPlaylistButton(lv_event_t * e);
lv_obj_t * ui_SelectPlaylistButton;
void ui_event_PlaylistDown(lv_event_t * e);
lv_obj_t * ui_PlaylistDown;
lv_obj_t * ui_lblPlaylistDown;
lv_obj_t * ui_lblSelectPlaylist;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_LightsOnSwitch(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        LightsOnCallback(e);
    }
}
void ui_event_LightsDimmerSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        DimmerChangedCallback(e);
    }
}
void ui_event_WakeupOverrideButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        WakeupOverrideCallback(e);
    }
}
void ui_event_TvPowerSwitch(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TvPowerToggleCallback(e);
    }
}
void ui_event_TvVolumeSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        TvVolumeChangeCallback(e);
    }
}
void ui_event_PlaylistUp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        playlistUpCallback(e);
    }
}
void ui_event_SelectPlaylistButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        playlistSubmitCallback(e);
    }
}
void ui_event_PlaylistDown(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        playlistDownCallback(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
