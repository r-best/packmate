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
    Screen::init();
    printf("%s\n", error_msg.c_str());
    set_trackball_led(255, 255, 0);
}

bool ErrorScreen::update(InputState *input) {
    if (input->trackball.clicked) {
        printf("Error screen clicked, closing, screenmanager: %p\n", &screenManager);
        screenManager.pop();
    }
    return Screen::update(input);
}

void ErrorScreen::custom_render() {
    set_pen_color(255, 255, 0);
    draw_text(error_msg, 20, 20, 200);
}
