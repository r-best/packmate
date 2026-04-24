#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#include "src/hardware/inputs/trackball.h"
#include "src/screens/screen.h"
#include "src/screens/home/home.h"
#include "src/screens/menu/menu.h"

class ScreenManager {
private:
    Screen *stack[4];
    uint8_t top = 0;
public:
    void push(Screen *s);
    void pop();
    Screen* active();
    bool update(InputState *input);
};

extern ScreenManager screenManager;

#endif
