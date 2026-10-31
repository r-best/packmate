#pragma once

#include <stdint.h>
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "common/pimoroni_bus.hpp"

namespace pimoroni {
    class ST7789 {
    public:
        uint16_t width, height;
        ST7789(uint16_t width, uint16_t height, Rotation rotation, bool round, SPIPins pins):
            width(width), height(height) {}

        void set_backlight(uint8_t brightness);
        void partial_update(PicoGraphics *graphics, Rect region);
        void update(PicoGraphics *graphics);
    };
}
