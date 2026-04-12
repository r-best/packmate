#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "libraries/breakout_trackball/breakout_trackball.hpp"

using namespace pimoroni;

static I2C i2c(BOARD::BREAKOUT_GARDEN, 100000); // Trackball requires I2C at 100kHz or less.

#endif
