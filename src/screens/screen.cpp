#include "screen.h"

const char* screen_name(ScreenID id) {
    switch (id) {
        case SCREEN_NULL: return "NULLScreen";
        case SCREEN_BOOT: return "BootScreen";
        case SCREEN_ERROR: return "ErrorScreen";
        case SCREEN_HOME: return "HomeScreen";
        case SCREEN_MENU: return "MenuScreen";
        default: return "ERROR fetching screen name, this should never happen";
    }
}
