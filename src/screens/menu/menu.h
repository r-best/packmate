#ifndef MENU_H
#define MENU_H

#include "widgets/button.h"
#include "widgets/memoryusage.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/hardware/display/lcd.h"

class MenuScreen: public EventScreen {
public:
    const char* name() const override { return "MenuScreen"; }

    static const uint8_t rows = 3;
    static const uint8_t cols = 3;

private:
    MemoryUsageBar *memoryUsageWidget;
    Button* buttons[rows*cols] = {nullptr};
    uint8_t selected = 0;

public:
    void init() override;
    bool shouldTriggerUpdate(InputState *input) override;
    void update(InputState *input) override;
    void render() override;

    ~MenuScreen() {
        for (int i = 0; i < 9; i++) {
            if (buttons[i]) delete buttons[i];
        }
    }

};

extern MenuScreen menuScreen;

#endif
