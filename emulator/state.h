#pragma once

#include <atomic>
#include <stdint.h>
#include <mutex>

#include "emulator/fakes/hardware/structs/watchdog.h"

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 240;

struct EmulatorState {
    // Screen
    std::mutex fb_mutex;
    uint16_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    std::atomic<bool> fb_dirty{false};

    // Buzzer
    std::atomic<float> buzzer_hz{0};

    // Trackball output
    std::mutex led_mutex;
    uint8_t matrix[5][5][3];
    uint8_t trackball_led[4]; // rgbw
    std::atomic<bool> leds_dirty{false};
    // Trackball input
    std::atomic<int>  tb_up{0}, tb_down{0}, tb_left{0}, tb_right{0};
    std::atomic<bool> tb_pressed{false};

    // Watchdog registers
    watchdog_hw_t     watchdog_regs{};
    std::atomic<bool> watchdog_rebooted{false};

    // Emulator management
    std::atomic<bool> stop_requested{false}; // Set for hot reload
};
