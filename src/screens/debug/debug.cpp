#include "debug.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"

void DebugScreen::init() {
    Screen::init();

    memoryUsageWidget = new MemoryUsageBar();
    memoryUsageWidget->x = 10;
    memoryUsageWidget->y = LCD::SCREEN_HEIGHT - 20;
    memoryUsageWidget->set_w(LCD::SCREEN_WIDTH - 20);
    memoryUsageWidget->h = 10;
    widgets.push_back(memoryUsageWidget);
}

bool DebugScreen::update(InputState *input) {
    if (input->trackball.clicked) {
        screenManager.pop();
        return false;
    }
    return Screen::update(input);
}
