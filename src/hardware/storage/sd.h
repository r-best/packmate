#ifndef SD_H
#define SD_H

#include <stdio.h>
#include "pico/stdlib.h"

#define SPRITE_LIMIT_240 5
#define SPRITE_LIMIT_128 50
#define SPRITE_LIMIT_64  100

#define SPRITE_SIZE_240 (240 * 240)
#define SPRITE_SIZE_128 (128 * 128)
#define SPRITE_SIZE_64  (64 * 64)

namespace SD {
    struct Sprite {
        uint8_t *data;
        uint8_t width; // frame width in pixels (same as height, sprites are squares)
        uint8_t frame_count;
        uint32_t size; // total bytes (width * width * frame_count)
        Sprite(): data(nullptr), width(0), frame_count(1), size(0) {}
        Sprite(uint8_t *data, uint8_t width, uint8_t frame_count, int size):
            data(data), width(width), frame_count(frame_count), size(size) {}
    };

    struct SpriteSlot {
        uint8_t *data; // Points to a space in one of the sprite memory banks over in sd.cpp
        bool loaded;
        char filename[64];
        uint8_t ref_count;
        uint32_t last_access_us;
        Sprite sprite;
    };
    struct SpritePool {
        SpriteSlot slots240[SPRITE_LIMIT_240];
        SpriteSlot slots128[SPRITE_LIMIT_128];
        SpriteSlot slots64[SPRITE_LIMIT_64];
    };

    int init();
    Sprite* load_sprite(const char *filename, uint8_t frame_width = 0, uint8_t frame_count = 1);
    void release_sprite(Sprite *sprite);
}

#endif
