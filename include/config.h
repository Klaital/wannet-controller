//
// Created by Kit on 9/11/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr pin_size_t BACKLIGHT_SWITCH_ON_PIN = A2;
constexpr pin_size_t ROTARY_ENCODER_CLK_PIN = 8;
constexpr pin_size_t ROTARY_ENCODER_DATA_PIN = 9;
constexpr pin_size_t ROTARY_ENCODER_BTN_PIN = A0;

constexpr pin_size_t BUZZER_PIN = A1;

constexpr char BEDROOM_TV_HOST[] = "192.168.88.178";
constexpr int BEDROOM_TV_PORT = 8080;

constexpr char LIGHTS_CONTROLLER_HOST[] = "192.168.88.42";
constexpr char MQTT_BROKER_HOST[] = "klaital.com";
constexpr int MQTT_BROKER_PORT = 1883;
constexpr char BEDROOM_DIMMER_TOPIC[] = "bedroom/lights/dimmer";
constexpr char BEDROOM_WAKEUP_TOPIC[] = "bedroom/lights/wakeup";
// Used to request the tv play a specified playlist
constexpr char BEDROOM_PLAYLIST_TOPIC[] = "bedroom/tv/playlist";
// Used by the TV controller to broadcast its settings
constexpr char BEDROOM_CONFIG_TOPIC[] = "bedroom/tv/config";
// Used to request the TV controller to broadcast its settings on the /config topic
constexpr char BEDROOM_UPDATE_TOPIC[] = "bedroom/tv/update";
// Used by the TV controller to broadcast the now-playing data - current filename and progress
constexpr char BEDROOM_NOWPLAYING_TOPIC[] = "bedroom/tv/nowplaying";



#endif //CONFIG_H
