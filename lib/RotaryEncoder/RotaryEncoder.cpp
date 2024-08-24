//
// Created by Kit on 8/22/2024.
//

#include "RotaryEncoder.h"

// singleton instance used for handling interrupts
RotaryEncoder Encoder;

void RotaryEncoder::begin(const pin_size_t clk, const pin_size_t data, const pin_size_t sw) {
    this->clk = clk;
    this->data = data;
    this->sw = sw;
    pinMode(this->clk, INPUT_PULLUP);
    pinMode(this->data, INPUT_PULLUP);
    pinMode(this->sw, INPUT_PULLDOWN);
    // TODO: refactor this to use interrupts rather than polling the lines
    this->clk_last = digitalRead(this->clk);
    // Serial.print("Registering clock interrupt on: ");
    // Serial.println(this->clk);
    // attachInterrupt(digitalPinToInterrupt(this->clk), &handle_clk, RISING);
    // Serial.print("Registering click interrupt on: ");
    // Serial.println(this->sw);
    // attachInterrupt(digitalPinToInterrupt(this->sw), handle_btn, RISING);
}

void RotaryEncoder::poll() {
    const PinStatus currentClk = digitalRead(this->clk);
    const unsigned long now = millis();
    if (currentClk != this->clk_last) {
        // knob is rotating
        if (now - this->rotate_last > this->rotate_cooldown) { // debounce the encoder
            this->rotate_last = now;
            if (digitalRead(this->data) != currentClk) {
                // means pin A changed first -> clockwise
                Serial.println("CW");
                if (this->cw_handler != nullptr) {
                    this->cw_handler();
                }
            } else {
                // means pin B changed first -> counter clockwise
                Serial.println("CCW");
                if (this->ccw_handler != nullptr) {
                    this->ccw_handler();
                }
            }
        }
    } else if (digitalRead(this->sw) == HIGH) { // check for button click
        // Also check the button

        if (now - this->button_last > this->button_cooldown) { // debounce the encoder
            this->button_last = now;
            if (this->btn_handler != nullptr) {
                this->btn_handler();
            }
        }
    }
    this->clk_last = currentClk;
}

void RotaryEncoder::register_btn_handler(const ButtonHandler h) {
    this->btn_handler = h;
}

void RotaryEncoder::register_cw_handler(const RotationHandler h) {
    this->cw_handler = h;
}

void RotaryEncoder::register_ccw_handler(const RotationHandler h) {
    this->ccw_handler = h;
}
