#ifndef HARDWARE_SCREEN_H
#define HARDWARE_SCREEN_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/hardware/storage/sd.h"

void screen_init();
void set_backlight(int brightness);
void draw_sprite(Sprite *sprite, uint8_t posx, uint8_t posy);
void draw_text(const char* msg, uint8_t posx, uint8_t posy, uint8_t wrap);
void clear_screen();
void update_screen();

#endif
