//
// Created by Kit on 8/22/2024.
//

#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

typedef void (*RotationHandler)(long pos);
typedef void (*ButtonHandler)();

class Encoder {
    pin_size_t clk, din, btn;
    PinStatus clk_last = LOW;
    long position = 0;
    unsigned long cooldown = 10ul;
    unsigned long rotate_last = 0;

    PinStatus btn_pos = LOW;
    unsigned long btn_last = 0;
    unsigned long btn_cooldown = 50ul;

    RotationHandler rotation_handler = nullptr, cw_handler = nullptr, ccw_handler = nullptr;
    ButtonHandler btn_handler = nullptr;

    bool bounds_configured = false;
    bool wrap = true; // if set to false, the values are capped at the min/max
    long min = 0, max=1;
public:
    explicit Encoder(const pin_size_t clk=0, const pin_size_t din=0, const pin_size_t btn=0): clk(clk), din(din), btn(btn) {}
    void begin(pin_size_t clk=0, pin_size_t din=0, pin_size_t btn=0);
    long read();

    void register_rotation_callback(RotationHandler h);
    void register_cw_callback(RotationHandler h);
    void register_ccw_callback(RotationHandler h);
    void register_btn_callback(ButtonHandler h);
    void configure_bounds(long min, long max, bool wrap=true); // if
};

#endif //ROTARYENCODER_H
