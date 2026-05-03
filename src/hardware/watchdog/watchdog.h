#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "src/screens/screen.h"

namespace watchdog {
    void start();
    bool is_watchdog_reboot();
    void feed();

    uint8_t get_active_screen();
    void set_active_screen(Screen *s);
}

#endif
