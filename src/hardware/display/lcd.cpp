#include "lcd.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "src/hardware/storage/sd.h"

#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

namespace LCD {
    const int SCREEN_WIDTH = 240;
    const int SCREEN_HEIGHT = 240;
    static uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    pimoroni::ST7789 st7789(SCREEN_WIDTH, SCREEN_HEIGHT, pimoroni::ROTATE_0, false, {spi1, 9U, 10U, 11U, pimoroni::PIN_UNUSED, 8U, 13U});
    pimoroni::PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, buffer);

    int init() {
        uint actual = spi_set_baudrate(spi1, 75000000);
        if (actual < 10000000) {
            printf("SPI is being limited\n");
            return -1;
        }

        // st7789.set_backlight(128); // Backlight control is CANCELLED until I can fix the flickering issue (too high power draw)
        graphics.clear();
        st7789.update(&graphics);
        return 0;
    }

    void set_backlight(int brightness) {
        st7789.set_backlight(brightness);
    }

    void set_pen_color(uint8_t r, uint8_t g, uint8_t b) {
        graphics.set_pen(r, g, b);
    }

    void clear_screen() {
        graphics.set_pen(0, 0, 0);
        graphics.clear();
    }

    void update_screen() {
        st7789.update(&graphics);
    }

    void draw_sprite(SD::Sprite *sprite, uint8_t posx, uint8_t posy) {
        if(sprite->width == SCREEN_WIDTH) {
            memcpy(buffer, sprite->data, sprite->size);
            return;
        }

        for(int i = 0; i < sprite->width; i++){
            memcpy(&buffer[(posx+i)*SCREEN_WIDTH+posy], &sprite->data[i*sprite->width], sprite->width);
        }
    }

    void draw_text(std::string msg, uint8_t posx, uint8_t posy, uint8_t wrap) {
        graphics.text(msg, pimoroni::Point(posx, posy), wrap);
    }

    void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h) {
        graphics.rectangle(pimoroni::Rect(x, y, w, h));
    }

    void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
        graphics.line(pimoroni::Point(x1, y1), pimoroni::Point(x2, y2));
    }
};
