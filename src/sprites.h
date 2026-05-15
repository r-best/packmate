#ifndef SPRITES_H
#define SPRITES_H

constexpr uint8_t CHROMA_KEY = 0xE3; // RGB332 magenta — treated as transparent

#define SPRITE_LIST \
    X(UI_BASE,       "sprites/ui_base.rgb332")       \
    X(HOME_MAIL,     "sprites/home/mail.rgb332")     \
    X(MENU_FOOD,     "sprites/menu/food.rgb332")     \
    X(MENU_PLAY,     "sprites/menu/play.rgb332")     \
    X(MENU_BACK,     "sprites/menu/back.rgb332")     \
    X(MENU_SOCIAL,   "sprites/menu/social.rgb332")   \
    X(MENU_SHOP,     "sprites/menu/shop.rgb332")     \
    X(MENU_SETTINGS, "sprites/menu/settings.rgb332") \

enum class SpriteID {
#define X(id, path) id,
    SPRITE_LIST
#undef X
};

constexpr const char* SPRITE_PATHS[] = {
#define X(id, path) path,
    SPRITE_LIST
#undef X
};

#endif
