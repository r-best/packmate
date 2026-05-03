#include "watchdog.h"

#include "hardware/watchdog.h"

namespace watchdog {
    const int MAGIC = 0xABBACAFE;

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
}
