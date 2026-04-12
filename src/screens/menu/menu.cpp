#include "menu.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screen.h"

MenuScreen menuScreen;

void MenuScreen::init() {
    ui_sprite = load_sprite("sprites/ui_base.rgb332");
}

void MenuScreen::update(InputState *input) {
    if((*input).trackball.clicked) {
        screenManager.pop();
    }
}

void MenuScreen::render() {
    draw_sprite(ui_sprite, 0, 0);
    draw_text("this is the main menu", 50, 50, 60);
}
