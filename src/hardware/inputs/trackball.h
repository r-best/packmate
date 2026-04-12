#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <stdio.h>
#include "pico/stdlib.h"

struct TrackballState {
    bool clicked = false;
    uint8_t direction = -1;
};

void trackball_init();
void get_trackball_state(TrackballState *state);

#endif
