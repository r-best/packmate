#ifndef RGB_MATRIX_H
#define RGB_MATRIX_H

#include <stdio.h>
#include "pico/stdlib.h"

int rgb_matrix_init();
void rgb_matrix_set_brightness(uint8_t brightness);
void rgb_matrix_set_color(uint8_t r, uint8_t g, uint8_t b);
void rgb_matrix_set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

#endif
