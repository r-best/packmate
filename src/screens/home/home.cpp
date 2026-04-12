#include "home.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/screen.h"
#include "src/sd.h"

HomeScreen homeScreen;

void HomeScreen::init() {
    ui_sprite = load_sprite("sprites/ui_base.rgb332");
}

void HomeScreen::update(InputState *input) {
    if(input->trackball.clicked) {
        screenManager.push(&menuScreen);
    }
}

void HomeScreen::render() {
    draw_sprite(ui_sprite, 0, 0);
    draw_text("character sprite goes here", 20, 160, 60);
}
