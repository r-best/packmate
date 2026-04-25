#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/hardware/display/lcd.h"

class MenuScreen: public EventScreen {
public:
    const char* name() const override { return "MenuScreen"; }
private:

    void init() override;
    bool shouldTriggerUpdate(InputState *input) override;
    void update(InputState *input) override;
    void render() override;
};

extern MenuScreen menuScreen;

#endif
