//
// Created by Kit on 10/16/2024.
//

#include <Arduino.h>
#include <config.h>
#include <MqttClient.h>
#include <TvConfig.h>
#include <ui.h>

extern MqttClient mqtt;

void UpdateNowPlaying(const char *title) {
    Serial.print("Updating now playing: ");
    Serial.println(title);
    lv_label_set_text_fmt(ui_NowPlaying, "Now Playing: %s", title);
}
void RequestTvConfigs() {
    mqtt.beginMessage(BEDROOM_UPDATE_TOPIC, 0, false);
    mqtt.print("1"); // the contents of the message are ignored - just the fact that a message is sent triggers the server code
    mqtt.endMessage();
}

extern TvConfig tv_config;
void HandleTvConfig(const String& raw) {
    char buffer[2048];
    strcpy(buffer, raw.c_str());
    tv_config.parse(buffer);
    lv_roller_set_options(ui_PlaylstSelection, tv_config.playlist_options, LV_ANIM_ON);
    // TODO: also set the selected option
}
