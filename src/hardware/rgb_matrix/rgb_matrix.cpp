#include "rgb_matrix.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "libraries/breakout_rgbmatrix5x5/breakout_rgbmatrix5x5.hpp"
#include "common/pimoroni_common.hpp"

#include "src/hardware/hardware.h"

namespace RGBMatrix {
    pimoroni::BreakoutRGBMatrix5x5 rgbmatrix5x5(&i2c);

    static uint8_t rgb_matrix_brightness = 255;
    static uint8_t rgb_matrix_pixels[5][5][3] = {{{0}}};

    static uint8_t scale_brightness(uint8_t color_value) {
        return (uint32_t(color_value) * rgb_matrix_brightness + 127) / 255;
    }

    static void update_pixel(uint8_t x, uint8_t y) {
        rgbmatrix5x5.set_pixel(x, y,
            scale_brightness(rgb_matrix_pixels[x][y][0]),
            scale_brightness(rgb_matrix_pixels[x][y][1]),
            scale_brightness(rgb_matrix_pixels[x][y][2])
        );
    }

    static void update_all() {
        for(char x = 0; x < 5; x++) {
            for(char y = 0; y < 5; y++) {
                update_pixel(x, y);
            }
        }
        rgbmatrix5x5.update(0);
    }

    int init() {
        if(!rgbmatrix5x5.init()) {
            printf("RGB matrix failed to init\n");
            return -1;
        }
        set_brightness(30);
        set_color(255, 255, 255);
        return 0;
    }

    void set_brightness(uint8_t brightness) {
        rgb_matrix_brightness = brightness;
        update_all();
    }

    void set_color(uint8_t r, uint8_t g, uint8_t b) {
        for(char x = 0; x < 5; x++) {
            for(char y = 0; y < 5; y++) {
                rgb_matrix_pixels[x][y][0] = r;
                rgb_matrix_pixels[x][y][1] = g;
                rgb_matrix_pixels[x][y][2] = b;
            }
        }
        update_all();
    }

    void set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
        rgb_matrix_pixels[x][y][0] = r;
        rgb_matrix_pixels[x][y][1] = g;
        rgb_matrix_pixels[x][y][2] = b;
        update_pixel(x, y);
        rgbmatrix5x5.update(0);
    }
}
