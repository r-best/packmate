#include "screen_manager.h"

#include <stdio.h>
#include "pico/stdlib.h"

ScreenManager screenManager;

void ScreenManager::push(Screen *s) {
    printf("Pushing screen %s\n", s->name());
    stack[++top] = s;
    clear_screen();
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
    delete current;
    active()->markStale();
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

    bool doRender = current->update(input);
    if (doRender) {
        // Need to fetch active screen again in case the update caused a screen change
        active()->render();
    }
    return doRender;
}
