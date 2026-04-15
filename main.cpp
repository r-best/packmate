#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"

#include "src/screens/screen_manager.h"
#include "src/screens/home/home.h"
#include "src/hardware/inputs/trackball.h"
#include "src/hardware/rgb_matrix/rgb_matrix.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"
#include "src/hardware/sound/buzzer.h"

#define DEBUG

#ifdef DEBUG
#include "tusb.h"
#endif

void core1_main() {
    printf("Second core initialized\n");

    printf("Mounting SD card\n");
    mount_sd();

    printf("Initializing buzzer\n");
    buzzer_init();
    play_melody("boot");

    // Initialise the Wi-Fi chip
    printf("Initializing wifi chip\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
}

int main() {
    stdio_init_all();

    #ifdef DEBUG
    // Wait for terminal connection (with 5s timeout)
    for (int i = 0; i < 500; i++) {
        if (tud_cdc_connected()) break;
        sleep_ms(10);
    }
    sleep_ms(200);
    #endif

    printf("------------------------------------\n");
    printf("Starting Packmate!\n");

    multicore_launch_core1(core1_main);

    printf("Initializing screen\n");
    screen_init();

    // Initialize RGB matrix
    printf("Initializing RGB matrix\n");
    rgb_matrix_init();
    
    printf("Initializing trackball\n");
    trackball_init();

    screenManager.push(&homeScreen);

    // Main loop
    InputState input;
    while(true){
        // Update inputs
        get_trackball_state(&input.trackball);

        // Update current screen
        screenManager.update(&input);
        update_screen();
    }
}
