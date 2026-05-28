#include "home.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"
#include "widgets/sprite_widget.h"

void HomeScreen::init() {
    Screen::init();
    Trackball::set_led(0, 255, 0);
    ui_sprite = loadSprite(SpriteID::UI_BASE);

    SpriteWidget *character = new SpriteWidget(SpriteID::CHARACTER, 0, 0, 2, true);
    character->x = character->y = (LCD::SCREEN_WIDTH - character->w) / 2;
    widgets.push_back(character);
}

bool HomeScreen::update(InputState *input) {
    if(input->trackball.clicked) {
        screenManager.push(new MenuScreen());
    }
    return Screen::update(input);
}

void HomeScreen::custom_render() {
    // LCD::draw_sprite(ui_sprite, 0, 0);
    LCD::clear_screen();
}
