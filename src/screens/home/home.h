#ifndef HOME_H
#define HOME_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class HomeScreen: public Screen {
public:
    const char* name() const override { return "HomeScreen"; }
private:
    Sprite *ui_sprite;

    void init() override;
    bool update(InputState *input) override;
    void custom_render() override;
};

#endif
