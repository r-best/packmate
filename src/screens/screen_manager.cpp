#include "screen_manager.h"

#include <stdio.h>
#include "pico/stdlib.h"

ScreenManager screenManager;

void ScreenManager::push(Screen *s) {
    printf("Pushing screen %s\n", s->name());
    stack[++top] = s;
    s->push_time_us = time_us_32();
    s->init();
}

void ScreenManager::pop() {
    Screen *current = active();
    printf("Popping screen %s\n", current ? current->name() : "null");
    if (current != nullptr) {
        current->unload();
    }
    if (top > 0) top--;
}

Screen* ScreenManager::active() {
    return top > 0 ? stack[top] : nullptr;
}

bool ScreenManager::update(InputState *input) {
    // Update current screen
    Screen *current = active();
    if (current == nullptr) {
        return false;
    }

    bool doUpdate = false;
    uint32_t now = time_us_32();
    switch (current->getUpdateMode()) {
        case FPS:
            doUpdate = true;
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
    return doUpdate;
}
