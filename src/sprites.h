#ifndef SPRITES_H
#define SPRITES_H

#include "pico/stdlib.h"

//      id                    path                   width frames
#define SPRITE_LIST \
    X(UI_BASE,       "sprites/ui_base.rgb332",        240,   1) \
    X(HOME_MAIL,     "sprites/home/mail.rgb332",       64,   1) \
    X(MENU_FOOD,     "sprites/menu/food.rgb332",       64,   1) \
    X(MENU_PLAY,     "sprites/menu/play.rgb332",       64,   1) \
    X(MENU_BACK,     "sprites/menu/back.rgb332",       64,   1) \
    X(MENU_SOCIAL,   "sprites/menu/social.rgb332",     64,   1) \
    X(MENU_SHOP,     "sprites/menu/shop.rgb332",       64,   1) \
    X(MENU_SETTINGS, "sprites/menu/settings.rgb332",   64,   1) \

enum class SpriteID {
#define X(id, path, width, frames) id,
    SPRITE_LIST
#undef X
};

struct SpriteInfo {
    const char *path;
    uint8_t width;
    uint8_t frame_count;
};

constexpr SpriteInfo SPRITE_INFO[] = {
#define X(id, path, width, frames) { path, width, frames },
    SPRITE_LIST
#undef X
};

#endif
