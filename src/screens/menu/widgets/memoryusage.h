#ifndef MENU_MEMORYUSAGE_H
#define MENU_MEMORYUSAGE_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class MemoryUsageBar: public Widget {
public:
    MemoryUsageBar() {
        h = 10;
    }

    bool update(InputState *input) override;
    void render() override;
};

#endif
