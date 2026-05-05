#ifndef MENU_MEMORYUSAGE_H
#define MENU_MEMORYUSAGE_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class MemoryUsageBar: public Widget {
private:
    uint8_t bar_w;
public:
    MemoryUsageBar() {
        bar_w = w;
    }

    bool update(InputState *input) override;
    void render() override;
};

#endif
