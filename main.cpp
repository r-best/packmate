#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"

#include "src/hardware/watchdog/watchdog.h"
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

uint8_t MAIN_FPS = 20;
uint32_t FRAME_INTERVAL_US = 1000000 / MAIN_FPS; // Storing this as microseconds because the timer function returns microseconds
uint32_t WAIT_INTERVAL_MS = FRAME_INTERVAL_US / 4 / 1000; // Wait 1/4 of frame interval between loops to save CPU, no need to check constantly. Could probably even drop to 1/2

void set_fps(uint8_t fps) {
    MAIN_FPS = fps;
    FRAME_INTERVAL_US = 1000000 / MAIN_FPS;
}

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

    sleep_ms(500);

    printf("Initializing buzzer\n");
    status = buzzer_init();
    bootScreen->update_status(3, status==0);

    sleep_ms(500);

    printf("Mounting SD card\n");
    status = mount_sd();
    bootScreen->update_status(4, status==0);

    sleep_ms(500);

    // Initialise the Wi-Fi chip
    printf("Initializing wifi chip\n");
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        bootScreen->update_status(5, false);
        return -1;
    }
    bootScreen->update_status(5, true);

    sleep_ms(500);

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

    screenManager.push(new HomeScreen());

    if (watchdog::is_watchdog_reboot()) {
        printf("Rebooted by watchdog - there was some kind of error\n");
        screenManager.push(new ErrorScreen("Watchdog reboot - Press any button to continue"));
    }

    screenManager.push(bootScreen);

    watchdog::start();

    // Main loop
    uint32_t lastUpdateTime = time_us_32();
    while(true){
        watchdog::feed();

        uint32_t now = time_us_32();
        if (now - lastUpdateTime >= FRAME_INTERVAL_US) {
            lastUpdateTime = now;

            // Update inputs
            get_trackball_state(&input.trackball);

            // Update current screen
            bool updated = screenManager.update(&input);
            if (updated) {
                update_screen();
            }
        }
        sleep_ms(WAIT_INTERVAL_MS);
    }
}
