#ifndef SD_H
#define SD_H

#include <stdio.h>
#include "pico/stdlib.h"

#define SPRITE_LIMIT_240 5
#define SPRITE_LIMIT_128 50
#define SPRITE_LIMIT_64  100

#define SPRITE_SIZE_240 (240 * 240 * 2)
#define SPRITE_SIZE_128 (128 * 128 * 2)
#define SPRITE_SIZE_64  (64 * 64 * 2)

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

    int init();
    Sprite* load_sprite(const char *filename, uint8_t frame_width, uint8_t frame_count);
    void release_sprite(Sprite *sprite);
}

#endif
