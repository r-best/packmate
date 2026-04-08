#include "screen.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "sd.h"

#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

using namespace pimoroni;

const int WIDTH = 240;
const int HEIGHT = 240;

ST7789 st7789(WIDTH, HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_BACK));
PicoGraphics_PenRGB565 graphics(st7789.width, st7789.height, nullptr);


void screen_init() {
    st7789.set_backlight(255);
    graphics.clear();

    // char buf[1700];
    printf("Hey\n");
    // int x = read_file("sprites/battery.rgb332", buf, sizeof(buf));
    // printf(buf);
    printf("\n");


    graphics.sprite(nullptr, Point(0, 0), Point(100, 100), 10.0f, 1);
    printf("1\n");


    st7789.update(&graphics);
    printf("2\n");
}

void set_backlight(int brightness) {
    st7789.set_backlight(brightness);
}
