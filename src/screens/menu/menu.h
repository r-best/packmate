#ifndef MENU_H
#define MENU_H

#include "widgets/button.h"
#include "widgets/memoryusage.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/hardware/display/lcd.h"

class MenuScreen: public Screen {
public:
    const char* name() const override { return "MenuScreen"; }

    static const uint8_t rows = 3;
    static const uint8_t cols = 3;

    void init() override;
    bool update(InputState *input) override;

private:
    MemoryUsageBar *memoryUsageWidget;
    Button* buttons[rows*cols] = {nullptr};
    uint8_t selected = 4;
};

#endif
