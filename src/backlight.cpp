//
// Created by Kit on 9/10/2024.
//

#include "Arduino_GigaDisplay.h"
extern GigaDisplayBacklight backlight;
extern bool backlight_toggle_requested;
extern pin_size_t BACKLIGHT_ON_PIN;
extern pin_size_t BACKLIGHT_OFF_PIN;
void BacklightSwitchInterrupt() {
    backlight_toggle_requested = true;
}
void init_backlight_handler() {
    pinMode(BACKLIGHT_ON_PIN, INPUT_PULLDOWN);
    pinMode(BACKLIGHT_OFF_PIN, INPUT_PULLDOWN);
    // attachInterrupt(BACKLIGHT_ON_PIN, BacklightSwitchInterrupt, RISING);
    // attachInterrupt(BACKLIGHT_OFF_PIN, BacklightSwitchInterrupt, RISING);
}

void DoBacklightChanged() {
    if (digitalRead(BACKLIGHT_ON_PIN) == HIGH) {
        backlight.set(100);
    } else {
        backlight.off();
    }
}
