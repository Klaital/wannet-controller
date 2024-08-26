//
// Created by Kit on 8/22/2024.
//

#include "RotaryEncoder.h"


void Encoder::begin(const pin_size_t clk, const pin_size_t data, const pin_size_t sw) {
    this->clk = clk;
    this->din = data;
    this->btn = sw;
    pinMode(this->clk, INPUT_PULLUP);
    pinMode(this->din, INPUT_PULLUP);
    pinMode(this->btn, INPUT_PULLDOWN);
    // TODO: refactor this to use interrupts rather than polling the lines
    this->clk_last = digitalRead(this->clk);
    // Serial.print("Registering clock interrupt on: ");
    // Serial.println(this->clk);
    // attachInterrupt(digitalPinToInterrupt(this->clk), &handle_clk, RISING);
    // Serial.print("Registering click interrupt on: ");
    // Serial.println(this->sw);
    // attachInterrupt(digitalPinToInterrupt(this->sw), handle_btn, RISING);
}

long Encoder::read() {
    const unsigned long now = millis();
    // Check the button
    if (now - this->btn_last > this->btn_cooldown) {
        // cooldown has expired
        this->btn_last = now;
        const PinStatus btn_new_pos = digitalRead(this->btn);
        if (btn_new_pos != this->btn_pos) {
            if (btn_new_pos == HIGH && this->btn_handler != nullptr) {
                // invoke the button-press handling code
                this->btn_handler();
            }
            this->btn_pos = btn_new_pos;
        }
    }

    // early-out if we're spinning too fast.
    // This is an ultra-simple debouncer.
    if (now - this->rotate_last < this->cooldown) {
        return this->position;
    }
    const PinStatus currentClk = digitalRead(this->clk);
    if (currentClk == this->clk_last) {
        // encoder hasn't moved, so early-out
        return this->position;
    }
    this->clk_last = currentClk;
    if (currentClk == HIGH) {
        // skip rising edges
        return this->position;
    }
    const PinStatus data = digitalRead(this->din);

    // Dial is moving
    // Serial.print("Encoder is moving ");
    if (data == HIGH) {
        // Serial.println("CW");
        // moving cw

        // Increment the position
        this->position++;
        if (bounds_configured && this->position > this->max) {
            if (wrap) {
                // wrap around to the beginning of the set
                this->position = this->min;
            } else {
                // cap value at the end of the set
                this->position = this->max;
            }
        }
        if (this->rotation_handler != nullptr) {
            this->rotation_handler(this->position);
        }
        if (this->cw_handler != nullptr) {
            this->cw_handler(this->position);
        }
    } else {
        // moving counterclockwise
        this->position--;
        if (bounds_configured && this->position < this->min) {
            if (wrap) {
                // wrap around to the endof the set
                this->position = this->max;
            } else {
                // cap value at the beginning of the set
                this->position = this->min;
            }
        }
        if (this->rotation_handler != nullptr) {
            this->rotation_handler(this->position);
        }
        if (this->ccw_handler != nullptr) {
            this->ccw_handler(this->position);
        }
    }
    return this->position;
}

// void RotaryEncoder::poll() {
//
//     const unsigned long now = millis();
//     const unsigned long time_since_rotation = now - this->rotate_last;
//     const unsigned long time_since_click = now - this->button_last;
//     if (currentClk != this->clk_last) {
//         // knob is rotating
//         // Serial.print("Rotation timing. Now=");
//         // Serial.print(now);
//         // Serial.print(", last rotation = ");
//         // Serial.print(this->rotate_last);
//         // Serial.print(", delta = ");
//         // Serial.println(time_since_rotation);
//         rotation_count++;
//         Serial.print(rotation_count);
//         Serial.print(" ");
//         if (time_since_rotation > this->rotate_cooldown) { // debounce the encoder
//             this->rotate_last = now;
//             if (digitalRead(this->data) != currentClk) {
//                 // means pin A changed first -> clockwise
//                 if (this->cw_handler != nullptr) {
//                     this->cw_handler();
//                 }
//             } else {
//                 // means pin B changed first -> counter clockwise
//                 if (this->ccw_handler != nullptr) {
//                     this->ccw_handler();
//                 }
//             }
//         }
//     } else if (digitalRead(this->sw) == HIGH) { // check for button click
//         // Also check the button
//
//         if (time_since_click > this->button_cooldown) { // debounce the encoder
//             this->button_last = now;
//             if (this->btn_handler != nullptr) {
//                 this->btn_handler();
//             }
//         }
//     }
//     this->clk_last = currentClk;
// }

void Encoder::register_btn_callback(const ButtonHandler h) {
    this->btn_handler = h;
}

void Encoder::register_cw_callback(const RotationHandler h) {
    Serial.println("Registering CW handler");
    if (h == nullptr) {
        Serial.println("It's null!");
    }
    this->cw_handler = h;

}

void Encoder::register_ccw_callback(const RotationHandler h) {
    this->ccw_handler = h;
}

void Encoder::register_rotation_callback(const RotationHandler h) {
    this->rotation_handler = h;
}


void Encoder::configure_bounds(long min, long max, bool wrap) {
    this->min = min;
    this->max = max;
    this->wrap = wrap;
    this->bounds_configured = true;
}
