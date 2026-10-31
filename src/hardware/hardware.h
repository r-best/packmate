#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "common/pimoroni_i2c.hpp"

inline pimoroni::I2C i2c(pimoroni::BOARD::BREAKOUT_GARDEN, 100000); // Trackball requires I2C at 100kHz or less.

#endif
