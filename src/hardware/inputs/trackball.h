#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <stdio.h>
#include "pico/stdlib.h"

struct TrackballState {
    bool clicked = false;
    uint8_t direction = -1;
};

int trackball_init();
void get_trackball_state(TrackballState *state);
void set_trackball_led(uint8_t r, uint8_t g, uint8_t b);

#endif
