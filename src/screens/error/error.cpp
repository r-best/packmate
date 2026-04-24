#include "error.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"

void ErrorScreen::init() {
    EventScreen::init();
    printf("%s\n", error_msg.c_str());
}

void ErrorScreen::update(InputState *input) {
    
}

void ErrorScreen::render() {
    set_pen_color(255, 0, 0);
    draw_text(error_msg, 20, 20, 200);
}
