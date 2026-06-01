#include "sprite_widget.h"

#include "pico/stdlib.h"
#include "src/hardware/lcd.h"
#include "src/hardware/sd.h"

SpriteWidget::SpriteWidget(SpriteID id, int16_t x, int16_t y, uint8_t scale, bool transparent, uint32_t frame_interval_us)
    : scale(scale), transparent(transparent), frame_interval_us(frame_interval_us), sprite(nullptr), frame(0), last_anim_us(time_us_32()) {
    this->x = x;
    this->y = y;
    sprite = loadSprite(id);
    if (sprite) {
        w = h = sprite->width * scale;
    }
}

bool SpriteWidget::update(InputState *input) {
    if (sprite && frame_interval_us > 0 && sprite->frame_count > 1) {
        uint32_t now = time_us_32();
        if (now - last_anim_us >= frame_interval_us) {
            frame = (frame + 1) % sprite->frame_count;
            last_anim_us = now;
            stale = true;
        }
    }
    return Widget::update(input);
}

void SpriteWidget::render() {
    if (sprite) {
        LCD::draw_sprite(sprite, x, y, transparent, frame, scale);
    }
    stale = false;
}
