#include "trackball.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "drivers/trackball/trackball.hpp"
#include "libraries/breakout_trackball/breakout_trackball.hpp"

#include "src/hardware/hardware.h"

namespace Trackball {
    pimoroni::BreakoutTrackball trackball(&i2c);
    
    static uint8_t SENSITIVITY = 2;
    static int LONG_PRESS_US = 1000000;
    static uint32_t press_start_us = 0; // Tracks time button is held (for long press)
    static bool long_press_fired = false; // Flag to avoid click triggering when long press is released

    int init() {
        if(!trackball.init()) {
            printf("Trackball failed to init!\n");
            return -1;
        }
        trackball.set_rgbw(0, 255, 0, 255);
        return 0;
    }

    void get_state(TrackballState *state) {
        pimoroni::Trackball::State newState = trackball.read();

        state->clicked = false;
        state->long_pressed = false;

        if (newState.sw_changed && newState.sw_pressed) {
            // Button was just clicked, start timer for long press
            // But nothing else yet, normal click doesn't trigger until release
            press_start_us = time_us_32();
        } else if (newState.sw_pressed && !long_press_fired) {
            // Still holding, check if long enough to trigger long press
            if (time_us_32() - press_start_us >= LONG_PRESS_US) {
                state->long_pressed = true;
                long_press_fired = true;
            }
        } else if (newState.sw_changed && !newState.sw_pressed) {
            // Button was released, if we already did a long press then ignore
            // Otherwise click
            if (!long_press_fired) {
                state->clicked = true;
            }
            long_press_fired = false;
        }

        if(newState.up > SENSITIVITY)           state->direction = 0;
        else if(newState.right > SENSITIVITY)   state->direction = 1;
        else if(newState.down > SENSITIVITY)    state->direction = 2;
        else if(newState.left > SENSITIVITY)    state->direction = 3;
        else                                    state->direction = -1;
    }

    void set_led(uint8_t r, uint8_t g, uint8_t b) {
        trackball.set_rgbw(r, g, b, 255);
    }
}
