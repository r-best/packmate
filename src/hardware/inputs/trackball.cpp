#include "trackball.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "drivers/trackball/trackball.hpp"
#include "libraries/breakout_trackball/breakout_trackball.hpp"

#include "src/hardware/hardware.h"

static uint8_t SENSITIVITY = 2;

BreakoutTrackball trackball(&i2c);

int trackball_init() {
    if(!trackball.init()) {
        printf("Trackball failed to init!\n");
        return -1;
    }
    trackball.set_rgbw(0, 255, 0, 255);
    return 0;
}

void get_trackball_state(TrackballState *state) {
    Trackball::State newState = trackball.read();

    state->clicked = newState.sw_changed && newState.sw_pressed;

    if(newState.up > SENSITIVITY)           state->direction = 0;
    else if(newState.right > SENSITIVITY)   state->direction = 1;
    else if(newState.down > SENSITIVITY)    state->direction = 2;
    else if(newState.left > SENSITIVITY)    state->direction = 3;
    else                                    state->direction = -1;
}
