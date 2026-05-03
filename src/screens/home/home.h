#ifndef HOME_H
#define HOME_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class HomeScreen: public Screen {
public:
    SCREEN_ID(SCREEN_HOME);
    SCREEN_NAME(SCREEN_HOME);
private:
    Sprite *ui_sprite;

    void init() override;
    bool update(InputState *input) override;
    void custom_render() override;
};

#endif
