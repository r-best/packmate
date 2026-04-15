#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/hardware/display/lcd.h"

class MenuScreen: public Screen {
private:
    Sprite *ui_sprite;

    void init() override;
    void update(InputState *input) override;
    void render() override;
};

extern MenuScreen menuScreen;

#endif
