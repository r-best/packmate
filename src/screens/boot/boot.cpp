#include "boot.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "src/screens/screen_manager.h"
#include "src/screens/screen.h"
#include "src/screens/menu/menu.h"
#include "src/hardware/display/lcd.h"
#include "src/hardware/storage/sd.h"


void BootScreen::update_status(int8_t idx, bool succeeded) {
    statuses[idx] = succeeded ? 1 : -1;
    markStale();
}

void BootScreen::init() {
    EventScreen::init();
}

void BootScreen::update(InputState *input) {

}

void BootScreen::render() {
    set_pen_color(255, 255, 255);
    draw_text("Booting Packmate.....", 20, 20, 240);
    draw_text("--------------------------", 20, 30, 220);
    for (int i = 0; i < BOOT_ITEM_COUNT; i++) {
        uint8_t row_pos = 30+25*(i+1);

        switch (statuses[i]) {
            case -1:
                set_pen_color(255, 0, 0);
                draw_rect(25, row_pos, 15, 15); // Outer box edge
                set_pen_color(0, 0, 0);
                draw_rect(27, row_pos+2, 11, 11); // Hollow out box
                set_pen_color(255, 0, 0);
                // Draw red X in box
                draw_line(25, row_pos, 40, row_pos+15);
                draw_line(25, row_pos+14, 40, row_pos);
                break;
            case  0:
                set_pen_color(255, 255, 255);
                draw_rect(25, row_pos, 15, 15);
                set_pen_color(0, 0, 0);
                draw_rect(27, row_pos+2, 11, 11);
                set_pen_color(255, 255, 255);
                break;
            case  1:
                set_pen_color(0, 255, 0);
                draw_rect(25, row_pos, 15, 15);
                break;
            default: break;
        }

        draw_text(boot_items[i], 50, row_pos, 190);
    }
}
