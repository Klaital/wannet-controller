//
// Created by Kit on 8/19/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#define MQTT_BROKER_HOST "klaital.com"
#define MQTT_BROKER_PORT 1883
#define BEDROOM_DIMMER_TOPIC "bedroom/lights/dimmer"

constexpr pin_size_t ROTARY_ENCODER_CLK_PIN = 51;
constexpr pin_size_t ROTARY_ENCODER_DATA_PIN = 53;
constexpr pin_size_t ROTARY_ENCODER_BTN_PIN = 49;

#endif //CONFIG_H
