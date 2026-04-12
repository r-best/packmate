#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/hardware/inputs/trackball.h"
#include "src/screens/screen.h"
#include "src/screens/home/home.h"
#include "src/screens/menu/menu.h"

class ScreenManager {
private:
    Screen *stack[4];
    uint8_t top = -1;
public:
    void push(Screen *s) {
        stack[++top] = s;
        s->init();
    }

    void pop() {
        if (top >= 0) top--;
    }

    Screen* active() {
        return top >= 0 ? stack[top] : nullptr;
    }

    void update(InputState *input) {
        clear_screen();
        sleep_ms(30);

        // Update current screen
        Screen *current = active();
        if (current != nullptr) {
            current->update(input);
            current->render();
        }
    }
};

extern ScreenManager screenManager;

#endif
