#include "sd.h"

#ifndef SCREEN_H
#define SCREEN_H

void screen_init();
void set_backlight(int brightness);
void draw_sprite(Sprite *sprite, uint8_t posx, uint8_t posy);

#endif
