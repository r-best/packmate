#pragma once

#include <stdint.h>
#include "common/pimoroni_i2c.hpp"

namespace pimoroni {
    class BreakoutRGBMatrix5x5 {
    public:
        BreakoutRGBMatrix5x5(I2C *i2c) {}

        bool init();
        void set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
        void update(uint8_t frame);
    };
}
