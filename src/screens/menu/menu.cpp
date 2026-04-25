#include "menu.h"
#include "widgets/button.h"

#include <functional>
#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

MenuScreen menuScreen;

struct buttonDef {
    const char* label;
    Sprite* sprite;
    std::function<void()> onClick;
};

const uint8_t NUM_MENU_ITEMS = MenuScreen::rows * MenuScreen::cols;

void MenuScreen::init() {
    EventScreen::init();

    buttonDef MENU_ITEMS[NUM_MENU_ITEMS] = {
        {"Feed",        load_sprite("sprites/menu/food.rgb332"),        [this](){ printf("Feed button clicked\n"); } },
        {"Play",        load_sprite("sprites/menu/play.rgb332"),        [this](){ printf("Play button clicked\n"); } },
        {"Mail",        load_sprite("sprites/home/mail.rgb332"),        [this](){ printf("Mail button clicked\n"); } },
        {"Care",        nullptr,                                        [this](){ printf("Care button clicked\n"); } },
        {"Back",        load_sprite("sprites/menu/back.rgb332"),        [this](){ screenManager.pop(); }},
        {"Visitors",    load_sprite("sprites/menu/social.rgb332"),      [this](){ printf("Visitors button clicked\n"); } },
        {"",            nullptr,                                        [this](){ printf("Empty button clicked\n"); } },
        {"Shop",        load_sprite("sprites/menu/shop.rgb332"),        [this](){ printf("Shop button clicked\n"); } },
        {"Settings",    load_sprite("sprites/menu/settings.rgb332"),    [this](){ printf("Settings button clicked\n"); } }
    };

    int cols = 3;
    int rows = 3;

    int spacing = 10;
    int btn_w = 70;

    int start_x = 10;
    int start_y = 10;

    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        buttons[i] = new Button(MENU_ITEMS[i].label);

        buttons[i]->onClick = MENU_ITEMS[i].onClick;
        buttons[i]->sprite  = MENU_ITEMS[i].sprite;

        buttons[i]->x = start_x + (i%3) * (btn_w + spacing);
        buttons[i]->y = start_y + (i/3) * (btn_w + spacing);
        buttons[i]->w = btn_w;
        buttons[i]->h = btn_w;
    }
}

bool MenuScreen::shouldTriggerUpdate(InputState *input) {
    bool doUpdate = pendingUpdate;
    pendingUpdate = false;
    return doUpdate || input->trackball.clicked || input->trackball.direction != -1;
}

void MenuScreen::update(InputState *input) {
    int row = selected / cols;
    int col = selected % cols;
    switch (input->trackball.direction) {
    case 0: // UP
        if (row > 0) row--;
        break;
    case 1: // RIGHT
        if (col < cols - 1) col++;
        break;
    case 2: // DOWN
        if (row < rows - 1) row++;
        break;
    case 3: // LEFT
        if (col > 0) col--;
        break;
    default:
        break;
    }
    selected = row * cols + col;

    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        buttons[i]->focused = (i == selected);
    }

    if (input->trackball.clicked) {
        buttons[selected]->onClick();
    }
}

void MenuScreen::render() {
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        buttons[i]->render();
    }
}
