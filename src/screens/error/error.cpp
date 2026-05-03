#include "error.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/hardware/watchdog/watchdog.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"

ErrorScreen::ErrorScreen(std::string e): Screen() {
    error_msg = std::move(e);
    crash_screen_name = screen_name((ScreenID)watchdog::get_active_screen());
    crash_screen_name = "at [ " + crash_screen_name + " ]";
}

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
    // Draw red screen border
    const uint8_t BORDER_WIDTH = 5;
    const uint8_t TEXT_WRAP_WIDTH = 240;
    set_pen_color(255, 0, 0);
    draw_rect(0, 0, BORDER_WIDTH, SCREEN_HEIGHT);
    draw_rect(0, 0, SCREEN_WIDTH, BORDER_WIDTH);
    draw_rect(0, SCREEN_HEIGHT-BORDER_WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_rect(SCREEN_WIDTH-BORDER_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    set_pen_color(255, 255, 255);
    draw_text("AN ERROR OCCURRED", 10, 10, TEXT_WRAP_WIDTH);

    set_pen_color(255, 255, 0);
    draw_text(crash_screen_name, 10, 30, TEXT_WRAP_WIDTH);

    set_pen_color(255, 255, 255);
    draw_text("Error source:", 10, 70, TEXT_WRAP_WIDTH);
    set_pen_color(255, 0, 0);
    draw_text(error_msg, 10, 90, TEXT_WRAP_WIDTH);

    set_pen_color(0, 255, 0);
    draw_text("-- CLICK TO CONTINUE --", 10, 210, TEXT_WRAP_WIDTH);
}
