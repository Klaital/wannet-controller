//
// Created by Kit on 9/9/2024.
//

#include <Arduino.h>
#include <GigaDisplayRGB.h>

extern GigaDisplayRGB rgb;
void DisplayError(const char* msg) {
    // Blink the LED red and print the message
    Serial.print("ERROR: ");
    Serial.println(msg);
    rgb.on(255, 0, 0);
    delay(100);
    rgb.off();
    delay(100);
    rgb.on(255, 0, 0);
    rgb.off();
}
