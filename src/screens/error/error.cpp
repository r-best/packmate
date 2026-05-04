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
    Trackball::set_led(255, 255, 0);
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
    LCD::set_pen_color(255, 0, 0);
    LCD::draw_rect(0, 0, BORDER_WIDTH, LCD::SCREEN_HEIGHT);
    LCD::draw_rect(0, 0, LCD::SCREEN_WIDTH, BORDER_WIDTH);
    LCD::draw_rect(0, LCD::SCREEN_HEIGHT-BORDER_WIDTH, LCD::SCREEN_WIDTH, LCD::SCREEN_HEIGHT);
    LCD::draw_rect(LCD::SCREEN_WIDTH-BORDER_WIDTH, 0, LCD::SCREEN_WIDTH, LCD::SCREEN_HEIGHT);

    LCD::set_pen_color(255, 255, 255);
    LCD::draw_text("AN ERROR OCCURRED", 10, 10, TEXT_WRAP_WIDTH);

    LCD::set_pen_color(255, 255, 0);
    LCD::draw_text(crash_screen_name, 10, 30, TEXT_WRAP_WIDTH);

    LCD::set_pen_color(255, 255, 255);
    LCD::draw_text("Error source:", 10, 70, TEXT_WRAP_WIDTH);
    LCD::set_pen_color(255, 0, 0);
    LCD::draw_text(error_msg, 10, 90, TEXT_WRAP_WIDTH);

    LCD::set_pen_color(0, 255, 0);
    LCD::draw_text("-- CLICK TO CONTINUE --", 10, 210, TEXT_WRAP_WIDTH);
}
