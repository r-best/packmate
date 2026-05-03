#include "button.h"

#include <stdio.h>
#include "pico/stdlib.h"

bool Button::update(InputState *input) {
    if (focused && input->trackball.clicked) {
        if (onClick) onClick();
    }
    return Widget::update(input);
}

void Button::render() {
    // Background
    if (focused) {
        set_pen_color(255, 255, 255);   // white background when focused
    } else {
        set_pen_color(100, 100, 100);   // gray background otherwise
    }

    draw_rect(x, y, w, h);

    // Text color (invert for contrast)
    if (focused) {
        set_pen_color(0, 0, 0);         // black text on white
    } else {
        set_pen_color(255, 255, 255);   // white text on gray
    }

    // Approximate vertical centering
    int text_height = 8; // adjust if your font differs
    int text_y = y + (h - text_height) / 2;

    // Horizontal centering handled by width parameter
    draw_text(label, x, text_y, w);

    stale = false;
}
