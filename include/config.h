//
// Created by Kit on 9/11/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr pin_size_t BACKLIGHT_SWITCH_ON_PIN = A0;
constexpr pin_size_t ROTARY_ENCODER_CLK_PIN = 51;
constexpr pin_size_t ROTARY_ENCODER_DATA_PIN = 53;
constexpr pin_size_t ROTARY_ENCODER_BTN_PIN = 49;

constexpr char BEDROOM_TV_HOST[] = "192.168.88.178";
constexpr int BEDROOM_TV_PORT = 8080;

constexpr char LIGHTS_CONTROLLER_HOST[] = "192.168.88.42";
constexpr char MQTT_BROKER_HOST[] = "klaital.com";
constexpr int MQTT_BROKER_PORT = 1883;
constexpr char BEDROOM_DIMMER_TOPIC[] = "bedroom/lights/dimmer";


#endif //CONFIG_H
