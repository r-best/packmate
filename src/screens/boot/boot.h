#ifndef BOOT_SCREEN_H
#define BOOT_SCREEN_H

#include <stdio.h>
#include <string>
#include <map>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"


inline constexpr const char* boot_items[7] = {
    "display", "trackball", "rgb_matrix",
    "buzzer", "SD card", "wireless",
    "secrets",
};
const size_t BOOT_ITEM_COUNT = sizeof(boot_items) / sizeof(boot_items[0]);

class BootScreen: public EventScreen {
public:
    void update_status(int8_t idx, bool succeeded);

private:
    int8_t statuses[BOOT_ITEM_COUNT];

    void init() override;
    void update(InputState *input) override;
    void render() override;
};

#endif
