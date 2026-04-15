#include "rgb_matrix.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "libraries/breakout_rgbmatrix5x5/breakout_rgbmatrix5x5.hpp"

#include "src/hardware/hardware.h"

BreakoutRGBMatrix5x5 rgbmatrix5x5(&i2c);

int rgb_matrix_init() {
    if(!rgbmatrix5x5.init()) {
        printf("RGB matrix failed to init\n");
        return -1;
    }
    for(char x = 0; x < 5; x++) {
        for(char y = 0; y < 5; y++) {
            rgbmatrix5x5.set_pixel(x, y, 30, 30, 30);
        }
    }
    rgbmatrix5x5.update(0);
    return 0;
}
