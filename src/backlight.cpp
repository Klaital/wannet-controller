//
// Created by Kit on 9/11/2024.
//
#include <config.h>
#include <Arduino_GigaDisplay.h>

extern GigaDisplayBacklight Backlight;
extern volatile bool backlight_switch_changed;
extern byte BacklightBrightness;
void BacklightSwitchISR() {
    backlight_switch_changed = true;
}

// debounce the switch
unsigned long last_switched = 0;
void HandleBacklightSwitch() {
    const auto now = millis();
    if (now - last_switched > 10) {
        backlight_switch_changed = false;
        const PinStatus backlight_state = digitalRead(BACKLIGHT_SWITCH_ON_PIN);
        if (backlight_state == HIGH) {
            Backlight.begin();
            Backlight.set(BacklightBrightness); // TODO: make the backlight brightness configurable
        } else {
            Backlight.off();
        }
    }
}
