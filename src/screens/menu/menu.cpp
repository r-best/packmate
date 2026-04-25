#include "menu.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

MenuScreen menuScreen;

void MenuScreen::init() {
    EventScreen::init();
}

bool MenuScreen::shouldTriggerUpdate(InputState *input) {
    bool doUpdate = pendingUpdate;
    pendingUpdate = false;
    return doUpdate || input->trackball.clicked;
}

void MenuScreen::update(InputState *input) {
    if((*input).trackball.clicked) {
        screenManager.pop();
    }
}

void MenuScreen::render() {
    set_pen_color(255, 255, 255);
    draw_text("this is the main menu", 50, 50, 60);
}
