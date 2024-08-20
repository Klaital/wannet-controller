//
// Created by Kit on 8/19/2024.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <MqttClient.h>

#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "Arduino_GigaDisplay.h"

extern Arduino_H7_Video          Display;
extern Arduino_GigaDisplayTouch  TouchDetector;
extern GigaDisplayBacklight backlight;

extern MqttClient mqttClient;
extern String BedroomDimmerTopic;

#endif //DISPLAY_H
