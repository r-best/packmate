#include "watchdog.h"

#include "hardware/watchdog.h"

namespace watchdog {
    const uint32_t MAGIC = 0xABBACAFE;

    // -------------------------------------------------------------------------------
    // Utility functions for packing & unpacking 8 bit values from a 32 bit register
    // May be moved out to a utility package later
    uint32_t _pack(uint32_t &reg, uint8_t pos, uint8_t data) {
        const uint32_t SHIFT = pos * 8;
        const uint32_t MASK = 0xFFu << SHIFT;
        return (reg & ~MASK) | ((uint32_t)data << SHIFT);
    }

    uint8_t _unpack(uint32_t reg, uint8_t pos) {
        uint32_t SHIFT = pos * 8;
        uint32_t MASK  = 0xFFu << SHIFT;

        return (reg & MASK) >> SHIFT;
    }
    // -------------------------------------------------------------------------------

    void start() {
        // Set up the watchdog to reset after 5 seconds to catch any errors
        watchdog_enable(5000, 1);
    }

    bool is_watchdog_reboot(){
        bool ret = watchdog_caused_reboot() && watchdog_hw->scratch[0] == MAGIC;
        watchdog_hw->scratch[0] = MAGIC;
        return ret;
    }

    void feed() {
        watchdog_update();
    }

    uint8_t get_active_screen() {
        uint32_t reg = watchdog_hw->scratch[1];
        return _unpack(reg, 0);
    }

    void set_active_screen(Screen *s) {
        uint8_t screen_id = ScreenID::SCREEN_NULL;
        if (s != nullptr) screen_id = s->id();
        uint32_t reg = watchdog_hw->scratch[1]; // Set this in a variable before passing in because of volatile register weirdness
        watchdog_hw->scratch[1] = _pack(reg, 0, screen_id);
    }
}
