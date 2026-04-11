#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#include "drivers/trackball/trackball.hpp"
#include "libraries/breakout_trackball/breakout_trackball.hpp"

#include "libraries/breakout_rgbmatrix5x5/breakout_rgbmatrix5x5.hpp"

#include "src/screen.h"
#include "src/sd.h"
#include "src/buzzer.h"

#include "tusb.h"

using namespace pimoroni;

// Trackball Variables
static const uint8_t SENSITIVITY = 2;
I2C i2c(BOARD::BREAKOUT_GARDEN, 100000); // Trackball requires I2C at 100kHz or less.
BreakoutTrackball trackball(&i2c);

// RGB Matrix Variables
BreakoutRGBMatrix5x5 rgbmatrix5x5(&i2c);

void core1_main() {
    printf("Second core initialized\n");

    printf("Mounting SD card\n");
    mount_sd();

    printf("Initializing buzzer\n");
    buzzer_init();
    // play_melody("boot");

    // Initialise the Wi-Fi chip
    printf("Initializing wifi chip\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
}

int main()
{
    stdio_init_all();
    // Wait for terminal connection (with 5s timeout)
    for (int i = 0; i < 500; i++) {
        if (tud_cdc_connected()) break;
        sleep_ms(10);
    }
    sleep_ms(200);

    printf("------------------------------------\n");
    printf("Starting Packmate!\n");

    multicore_launch_core1(core1_main);

    printf("Initializing screen\n");
    screen_init();

    // Initialize RGB matrix
    printf("Initializing RGB matrix\n");
    rgbmatrix5x5.init();
    for(char x = 0; x < 5; x++) {
        for(char y = 0; y < 5; y++) {
            rgbmatrix5x5.set_pixel(x, y, 30, 30, 30);
        }
    }
    rgbmatrix5x5.update(0);
    
    printf("Initializing trackball\n");
    if(!trackball.init()) {
        printf("Trackball failed to init!\n");
        return 1;
    }

    trackball.set_rgbw(0, 0, 0, 64);
    while(true) {
        Trackball::State state = trackball.read();
        if(state.sw_pressed)                trackball.set_rgbw(0, 0, 0, 255);
        else if(state.left > SENSITIVITY)   trackball.set_rgbw(0, 0, 255, 0);
        else if(state.right > SENSITIVITY)  trackball.set_rgbw(255, 0, 0, 0);
        else if(state.up > SENSITIVITY)     trackball.set_rgbw(255, 255, 0, 0);
        else if(state.down > SENSITIVITY)   trackball.set_rgbw(0, 255, 0, 0);
        else if(state.sw_changed)           trackball.set_rgbw(0, 0, 0, 255);

        sleep_ms(20);
    }
}
