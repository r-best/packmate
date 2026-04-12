#include "screen.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "sd.h"

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

void screen_init() {
    spi_set_baudrate(spi1, 62500000);
    // st7789.set_backlight(128); // Backlight control is CANCELLED until I can fix the flickering issue (too high power draw)
    graphics.clear();
    st7789.update(&graphics);
}

void set_backlight(int brightness) {
    st7789.set_backlight(brightness);
}

void clear_screen() {
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

void draw_text(const char* msg, uint8_t posx, uint8_t posy, uint8_t wrap) {
    graphics.text(msg, Point(posx, posy), wrap);
}
