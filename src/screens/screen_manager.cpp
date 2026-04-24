#include "screen_manager.h"

#include <stdio.h>
#include "pico/stdlib.h"

ScreenManager screenManager;

void ScreenManager::push(Screen *s) {
    stack[++top] = s;
    s->init();
}

void ScreenManager::pop() {
    if (top > 0) top--;
}

Screen* ScreenManager::active() {
    return top > 0 ? stack[top] : nullptr;
}

void ScreenManager::update(InputState *input) {
    // Update current screen
    Screen *current = active();
    if (current == nullptr) {
        return;
    }

    bool doUpdate = false;
    uint32_t now = time_us_32();
    switch (current->getUpdateMode()) {
        case FPS:
            if (now - ((FPSScreen*)current)->lastUpdateTime >= ((FPSScreen*)current)->updateInterval_us) {
                doUpdate = true;
                ((FPSScreen*)current)->lastUpdateTime = now;
            }
            break;
        case EVENT:
            doUpdate = ((EventScreen*)current)->shouldTriggerUpdate(input);
            break;
    }
    if (doUpdate) {
        current->update(input);
        clear_screen();
        current->render();
    }
}
