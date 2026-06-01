#ifndef MENU_MEMORYUSAGE_H
#define MENU_MEMORYUSAGE_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/lcd.h"

class MemoryUsageBar: public Widget {
private:
    uint8_t bar_w;
public:
    MemoryUsageBar() {
        
    }

    void set_w(uint8_t width) {
        w = width;
        bar_w = w*.6;
    }

    bool update(InputState *input) override;
    void render() override;
};

#endif
