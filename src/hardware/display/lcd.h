#ifndef HARDWARE_SCREEN_H
#define HARDWARE_SCREEN_H

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"

#include "src/hardware/storage/sd.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

int screen_init();
void set_backlight(int brightness);
void set_pen_color(uint8_t r, uint8_t g, uint8_t b);
void draw_sprite(Sprite *sprite, uint8_t posx, uint8_t posy);
void draw_text(std::string msg, uint8_t posx, uint8_t posy, uint8_t wrap);
void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h);
void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void clear_screen();
void update_screen();

#endif
