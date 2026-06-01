#ifndef RGB_MATRIX_H
#define RGB_MATRIX_H

#include <stdio.h>
#include "pico/stdlib.h"

namespace RGBMatrix {
    int init();
    void set_brightness(uint8_t brightness);
    void set_color(uint8_t r, uint8_t g, uint8_t b);
    void set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
}

#endif
