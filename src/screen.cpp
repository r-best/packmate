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
static uint8_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

ST7789 st7789(SCREEN_WIDTH, SCREEN_HEIGHT, ROTATE_0, false, {spi1, 9U, 10U, 11U, PIN_UNUSED, 8U, 13U});
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, framebuffer);

void screen_init() {
    st7789.set_backlight(255);
    graphics.clear();

    printf("Reading UI sprite file\n");
    Sprite *ui = load_sprite("sprites/ui_base.rgb332");

    draw_sprite(ui, 0, 0);

    st7789.update(&graphics);
}

void draw_sprite(Sprite *sprite, uint8_t posx, uint8_t posy) {
    if(sprite->width == SCREEN_WIDTH) {
        memcpy(framebuffer, sprite->data, sprite->size);
        return;
    }

    for(int i = 0; i < sprite->width; i++){
        memcpy(&framebuffer[(posx+i)*SCREEN_WIDTH+posy], &sprite->data[i*sprite->width], sprite->width);
    }
}

void set_backlight(int brightness) {
    st7789.set_backlight(brightness);
}
