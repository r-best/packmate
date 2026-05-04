#include "screen_manager.h"

#include <stdio.h>
#include "pico/stdlib.h"

ScreenManager screenManager;

void ScreenManager::push(Screen *s) {
    printf("Pushing screen %s\n", s->name());
    if (top == SCREEN_STACK_SIZE-1) {
        printf("ERROR: screen stack full, cannot push");
    }
    stack[++top] = s;
    LCD::clear_screen();
}

void ScreenManager::pop() {
    Screen *current = active();
    if (current != nullptr) {
        to_delete.push_back(current);
    }
    if (top > 0) top--;
    LCD::clear_screen();
    if (active() != nullptr) {
        active()->markStale();
    }
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

    if (!current->is_initialized()) current->init();

    bool doRender = current->update(input);
    if (doRender) {
        // Need to fetch active screen again in case the update caused a screen change
        active()->render();
    }

    // Delete any screens marked for deletion
    for (auto s : to_delete) {
        delete s;
    }
    to_delete.clear();

    return doRender;
}
