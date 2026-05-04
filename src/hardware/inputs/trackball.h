#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <stdio.h>
#include "pico/stdlib.h"

namespace Trackball {
    struct TrackballState {
        bool clicked = false;
        uint8_t direction = -1;
    };

    int init();
    void get_state(TrackballState *state);
    void set_led(uint8_t r, uint8_t g, uint8_t b);
}

#endif
