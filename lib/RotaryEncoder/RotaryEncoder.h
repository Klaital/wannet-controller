//
// Created by Kit on 8/22/2024.
//

#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

typedef void (*RotationHandler)();
typedef void (*ButtonHandler)();

class RotaryEncoder {
public:
    pin_size_t clk;
    PinStatus clk_last;
    pin_size_t data;
    pin_size_t sw;
    unsigned long rotate_cooldown;
    unsigned long rotate_last;
    unsigned long button_cooldown;
    unsigned long button_last;

    RotationHandler cw_handler;
    RotationHandler ccw_handler;
    ButtonHandler btn_handler;

    RotaryEncoder() :
        clk(0), clk_last(LOW),
        data(0), sw(0),
        rotate_cooldown(150ul), rotate_last(0),
        button_cooldown(500ul), button_last(0),
        cw_handler(nullptr), ccw_handler(nullptr), btn_handler(nullptr){}

    void begin(pin_size_t clk, pin_size_t data, pin_size_t sw);
    void poll();
    void register_cw_handler(RotationHandler h);
    void register_ccw_handler(RotationHandler h);
    void register_btn_handler(ButtonHandler h);

};
#endif //ROTARYENCODER_H
