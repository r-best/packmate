#ifndef HOME_H
#define HOME_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class HomeScreen: public FPSScreen {
public:
    const char* name() const override { return "HomeScreen"; }
private:
    Sprite *ui_sprite;

    void init() override;
    void update(InputState *input) override;
    void render() override;
};

extern HomeScreen homeScreen;

#endif
