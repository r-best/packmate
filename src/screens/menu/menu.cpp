#include "menu.h"
#include "widgets/button.h"
#include "widgets/memoryusage.h"

#include <functional>
#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

struct buttonDef {
    const char* label;
    Sprite* sprite;
    std::function<void()> onClick;
};

const uint8_t NUM_MENU_ITEMS = MenuScreen::rows * MenuScreen::cols;

void MenuScreen::init() {
    Screen::init();

    memoryUsageWidget = new MemoryUsageBar();
    memoryUsageWidget->x = 10;
    memoryUsageWidget->y = SCREEN_HEIGHT - 20;
    memoryUsageWidget->w = SCREEN_WIDTH - 20;
    memoryUsageWidget->h = 10;
    widgets.push_back(memoryUsageWidget);

    buttonDef MENU_ITEMS[NUM_MENU_ITEMS] = {
        {"Feed",        loadSprite("sprites/menu/food.rgb332"),        [this](){ printf("Feed button clicked\n"); } },
        {"Play",        loadSprite("sprites/menu/play.rgb332"),        [this](){ printf("Play button clicked\n"); } },
        {"Mail",        loadSprite("sprites/home/mail.rgb332"),        [this](){ printf("Mail button clicked\n"); } },
        {"Care",        nullptr,                                        [this](){ printf("Care button clicked\n"); } },
        {"Back",        loadSprite("sprites/menu/back.rgb332"),        [this](){ screenManager.pop(); }},
        {"Visitors",    loadSprite("sprites/menu/social.rgb332"),      [this](){ printf("Visitors button clicked\n"); } },
        {"",            nullptr,                                        [this](){ printf("Empty button clicked\n"); } },
        {"Shop",        loadSprite("sprites/menu/shop.rgb332"),        [this](){ printf("Shop button clicked\n"); } },
        {"Settings",    loadSprite("sprites/menu/settings.rgb332"),    [this](){ printf("Settings button clicked\n"); } }
    };

    int cols = 3;
    int rows = 3;

    int spacing = 10;
    int btn_w = 50;

    int start_x = 20;
    int start_y = 20;

    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        buttons[i] = new Button(MENU_ITEMS[i].label);

        buttons[i]->onClick = MENU_ITEMS[i].onClick;
        buttons[i]->sprite  = MENU_ITEMS[i].sprite;

        buttons[i]->x = start_x + (i%3) * (btn_w + spacing);
        buttons[i]->y = start_y + (i/3) * (btn_w + spacing);
        buttons[i]->w = btn_w;
        buttons[i]->h = btn_w;

        widgets.push_back(buttons[i]);
    }
    buttons[selected]->set_focused(true);
}

bool MenuScreen::update(InputState *input) {
    int row = selected / cols;
    int col = selected % cols;

    bool updated = true;
    switch (input->trackball.direction) {
    case 0: // UP
        if (row > 0) row--;
        break;
    case 1: // RIGHT
        if (col < cols-1) col++;
        break;
    case 2: // DOWN
        if (row < rows-1) row++;
        break;
    case 3: // LEFT
        if (col > 0) col--;
        break;
    default:
        updated = false;
        break;
    }

    if (updated) {
        buttons[selected]->set_focused(false);
        selected = row * cols + col;
        buttons[selected]->set_focused(true);
    }

    return Screen::update(input);
}
