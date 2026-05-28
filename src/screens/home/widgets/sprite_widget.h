#ifndef SPRITE_WIDGET_H
#define SPRITE_WIDGET_H

#include "pico/stdlib.h"
#include "src/screens/screen.h"
#include "src/sprites.h"

class SpriteWidget : public Widget {
public:
    uint8_t scale;
    bool transparent;
    uint32_t frame_interval_us;

    SpriteWidget(SpriteID id, int16_t x, int16_t y, uint8_t scale = 1, bool transparent = false, uint32_t frame_interval_us = 125000);

    bool update(InputState *input) override;
    void render() override;

private:
    SD::Sprite *sprite;
    uint8_t frame;
    uint32_t last_anim_us;
};

#endif
