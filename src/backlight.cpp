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
void HandleBacklightSwitch() {
    backlight_switch_changed = false;
    const PinStatus backlight_state = digitalRead(BACKLIGHT_SWITCH_ON_PIN);
#ifdef DEBUG
    Serial.print("Backlight switched ");
    Serial.println(backlight_state);
#endif
    if (backlight_state == HIGH) {
        Backlight.begin();
        Backlight.set(BacklightBrightness); // TODO: make the backlight brightness configurable
    } else {
        Backlight.off();
    }
}
