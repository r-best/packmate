#pragma once

#include <stdint.h>
#include "common/pimoroni_i2c.hpp"

namespace pimoroni {
    class Trackball {
    public:
        struct State {
            uint8_t left;
            uint8_t right;
            uint8_t up;
            uint8_t down;
            bool sw_changed;
            bool sw_pressed;
        };

        Trackball(I2C *i2c) {}

        bool init();
        void set_rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
        State read();
    };
}
