#include "home.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"

void HomeScreen::init() {
    Screen::init();
    set_trackball_led(0, 255, 0);
    ui_sprite = loadSprite("sprites/ui_base.rgb332");
}

bool HomeScreen::update(InputState *input) {
    if(input->trackball.clicked) {
        screenManager.push(new MenuScreen());
    }
    return Screen::update(input);
}

void HomeScreen::custom_render() {
    draw_sprite(ui_sprite, 0, 0);
    draw_text("character sprite goes here", 20, 160, 60);
}
