#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class Button: public ClickableWidget {
public:
    const char* label;

    Button(const char* label) : label(label) {}

    void update(InputState *input) override;
    void render() override;
};

#endif
