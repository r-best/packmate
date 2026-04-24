#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"

#include "src/screens/screen_manager.h"
#include "src/screens/home/home.h"
#include "src/screens/error/error.h"
#include "src/screens/boot/boot.h"
#include "src/hardware/inputs/trackball.h"
#include "src/hardware/rgb_matrix/rgb_matrix.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"
#include "src/hardware/sound/buzzer.h"

#define DEBUG

#ifdef DEBUG
#include "tusb.h"
#endif

BootScreen *bootScreen;

int init_debug_connections() {
    stdio_init_all();

    // Wait for terminal connection (with 5s timeout)
    for (int i = 0; i < 500; i++) {
        if (tud_cdc_connected()) break;
        sleep_ms(10);
    }
    sleep_ms(200);
    return 0;
}

int init_secondary_hardware() {
    int status;

    printf("Initializing buzzer\n");
    status = buzzer_init();
    bootScreen->update_status(3, status==0);

    printf("Mounting SD card\n");
    status = mount_sd();
    bootScreen->update_status(4, status==0);

    // Initialise the Wi-Fi chip
    printf("Initializing wifi chip\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        bootScreen->update_status(5, false);
        return -1;
    }
    bootScreen->update_status(5, true);

    play_melody("boot");
    return 0;
}

void core1_main() {
    printf("Second core initialized\n");

    init_secondary_hardware();
}

int main() {
    #ifdef DEBUG
    init_debug_connections();
    #endif

    printf("------------------------------------\n");
    printf("Starting Packmate!\n");

    bootScreen = new BootScreen();

    printf("Initializing screen\n");
    int status = screen_init();
    bootScreen->update_status(0, status==0);
    
    // Try to initialize trackball first so we have the LED to light red on error
    printf("Initializing trackball\n");
    status = trackball_init();
    bootScreen->update_status(1, status==0);

    multicore_launch_core1(core1_main);

    // Initialize RGB matrix
    printf("Initializing RGB matrix\n");
    status = rgb_matrix_init();
    bootScreen->update_status(2, status==0);

    InputState input;

    // screenManager.push(&homeScreen);
    screenManager.push(bootScreen);
    screenManager.update(&input); // Not sure why this extra update needs to be here or it all breaks

    // Main loop
    while(true){
        // Update inputs
        get_trackball_state(&input.trackball);

        // Update current screen
        bool updated = screenManager.update(&input);
        if (updated) {
            update_screen();
        }
    }
}
