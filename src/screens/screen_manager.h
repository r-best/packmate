#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "pico/time.h"

#include "src/hardware/inputs/trackball.h"
#include "src/screens/screen.h"
#include "src/screens/home/home.h"
#include "src/screens/menu/menu.h"

#define SCREEN_STACK_SIZE 5

class ScreenManager {
private:
    Screen *stack[SCREEN_STACK_SIZE];
    uint8_t top = 0;

    // Screens that pop off the stack get stored here and deleted at the end of the next update loop
    // This prevents weird undefined C++ behavior from a screen deleting itself
    std::vector<Screen*> to_delete;
public:
    void push(Screen *s);
    void pop();
    Screen* active();
    bool update(InputState *input);
};

extern ScreenManager screenManager;

#endif
