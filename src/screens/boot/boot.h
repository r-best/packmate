#ifndef BOOT_SCREEN_H
#define BOOT_SCREEN_H

#include <stdio.h>
#include <string>
#include <map>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"


inline constexpr const char* boot_items[7] = {
    "[c0] display", "[c0] trackball", "[c0] rgb_matrix", // Core 0 items
    "[c1] buzzer", "[c1] SD card", "[c1] wireless", // Core 1 items
    "secrets",
};
const size_t BOOT_ITEM_COUNT = sizeof(boot_items) / sizeof(boot_items[0]);

class BootScreen: public EventScreen {
public:
    const char* name() const override { return "BootScreen"; }
    void update_status(int8_t idx, bool succeeded);

private:
    int8_t statuses[BOOT_ITEM_COUNT];

    void init() override;
    void update(InputState *input) override;
    void render() override;

    bool all_successful();
};

#endif
