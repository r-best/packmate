#ifndef WATCHDOG_H
#define WATCHDOG_H

namespace watchdog {
    void start();
    bool is_watchdog_reboot();
    void feed();
}

#endif
