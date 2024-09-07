// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: TabbedHomeUI

#include <TvControlClient.h>

#include "ui.h"
#include "TvConfig.h"

void LightsOnCallback(lv_event_t * e)
{
	const lv_event_code_t event_code = lv_event_get_code(e);
	if (event_code == LV_EVENT_CLICKED) {
		int level = 0;
		if (lv_obj_has_state(ui_LightsOnSwitch, LV_STATE_CHECKED)) {
			level = 100;
		}
		char label[5];
		lv_snprintf(label, sizeof label, "%d%%", level);
		_ui_label_set_property(ui_DimmerValueLabel, _UI_LABEL_PROPERTY_TEXT, label);
		lv_slider_set_value(ui_LightsDimmerSlider, level, LV_ANIM_ON);
	}
	// TODO: submit lights power update over MQTT
}

void DimmerChangedCallback(lv_event_t * e)
{
	const lv_event_code_t event_code = lv_event_get_code(e);
	const lv_obj_t * target = lv_event_get_target(e);
	char buf[8];
	auto dimmerValue = lv_slider_get_value(target);
	lv_snprintf(buf, sizeof buf, "%ld%%", dimmerValue);
	if(event_code == LV_EVENT_RELEASED) {
		_ui_label_set_property(ui_DimmerValueLabel, _UI_LABEL_PROPERTY_TEXT, buf);
		if (dimmerValue > 0) {
			lv_obj_add_state(ui_LightsOnSwitch, LV_STATE_CHECKED);
		} else {
			lv_obj_clear_state(ui_LightsOnSwitch, LV_STATE_CHECKED);
		}
	}
	// TODO: submit dimmer update over MQTT
}

extern TvControlClient tv_controller;
extern TvConfig tv_config;
extern bool updatedTvConfigRequested;
void TvPowerToggleCallback(lv_event_t * e)
{
	// Use the power-on function to also update the playlists supported
	if (lv_obj_has_state(ui_TvPowerSwitch, LV_STATE_CHECKED)) {
		// // Update the playlists
		// Serial.println("Fetching updated TV config");
		// tv_controller.FetchTvConfig(tv_config);
		// Serial.println("Got new playlist set:" );
		// Serial.println(tv_config.playlist_options);
		// lv_roller_set_options(ui_PlaylistSelectRoller, tv_config.playlist_options.c_str(), LV_ROLLER_MODE_NORMAL);
		// Serial.print("Selected Playlist: ");
		// Serial.print(tv_config.current_playlist);
		// Serial.print(" (");
		// Serial.print(tv_config.index_of_playlist(tv_config.current_playlist));
		// Serial.println(")");
		// lv_roller_set_selected(ui_PlaylistSelectRoller, tv_config.index_of_playlist(tv_config.current_playlist), LV_ANIM_ON);
		updatedTvConfigRequested = true;
	}
	// TODO: write an endpoint on the raspberry pi to control turning the TV on/off
}

void TvVolumeChangeCallback(lv_event_t * e)
{
	// TODO: write an endpoint on the raspberry pi to control the TV volume (or at least VLC's volume)
}

void WakeupOverrideCallback(lv_event_t * e)
{
	// TODO: write a service that can handle scheduling the wakeup calls. It's currently a cronjob on klaital.com, so not easily altered.
}

void playlistUpCallback(lv_event_t * e)
{
	lv_obj_t* roller = ui_PlaylistSelectRoller;
	auto v = lv_roller_get_selected(roller) - 1;
	if (v <= 0) {
		// cap at the top of the list
		v = 0;
	}
	lv_roller_set_selected(roller, v, LV_ANIM_ON);
}

extern bool changePlaylistRequested;
unsigned long last_playlist_change_time = 0;
void playlistSubmitCallback(lv_event_t * e)
{
	// debounce the button press
	const unsigned long now = millis();
	if (now - last_playlist_change_time > 250) {
		changePlaylistRequested = true;
		last_playlist_change_time = now;
	}
}

void playlistDownCallback(lv_event_t * e)
{
	lv_obj_t* roller = ui_PlaylistSelectRoller;
	auto v = lv_roller_get_selected(roller) + 1;
	auto playlistCount = lv_roller_get_option_cnt(roller);

	if (v >= playlistCount) {
		// cap at the bottom of the list
		v = playlistCount-1;
	}
	lv_roller_set_selected(roller, v, LV_ANIM_ON);
}
