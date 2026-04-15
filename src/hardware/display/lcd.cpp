#include "lcd.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "src/hardware/storage/sd.h"

#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

using namespace pimoroni;

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 240;
static uint8_t buffer1[SCREEN_WIDTH * SCREEN_HEIGHT];
static uint8_t buffer2[SCREEN_WIDTH * SCREEN_HEIGHT];
static uint8_t *front_buffer = buffer1;
static uint8_t *back_buffer = buffer2;

ST7789 st7789(SCREEN_WIDTH, SCREEN_HEIGHT, ROTATE_0, false, {spi1, 9U, 10U, 11U, PIN_UNUSED, 8U, 13U});
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, back_buffer);

int screen_init() {
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
    // Swap draw buffers
    uint8_t* temp = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp;
    graphics.set_framebuffer(back_buffer);

    st7789.update(&graphics);
}

void draw_sprite(Sprite *sprite, uint8_t posx, uint8_t posy) {
    if(sprite->width == SCREEN_WIDTH) {
        memcpy(back_buffer, sprite->data, sprite->size);
        return;
    }

    for(int i = 0; i < sprite->width; i++){
        memcpy(&back_buffer[(posx+i)*SCREEN_WIDTH+posy], &sprite->data[i*sprite->width], sprite->width);
    }
}

void draw_text(std::string msg, uint8_t posx, uint8_t posy, uint8_t wrap) {
    graphics.text(msg, Point(posx, posy), wrap);
}

void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h) {
    graphics.rectangle(Rect(x, y, w, h));
}

void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    graphics.line(Point(x1, y1), Point(x2, y2));
}
