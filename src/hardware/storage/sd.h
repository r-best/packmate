#ifndef SD_H
#define SD_H

#include <stdio.h>
#include "pico/stdlib.h"

#define SPRITE_LIMIT_240 1
#define SPRITE_LIMIT_128 10
#define SPRITE_LIMIT_64  10

#define SPRITE_SIZE_240 (240 * 240)
#define SPRITE_SIZE_128 (128 * 128)
#define SPRITE_SIZE_64  (64 * 64)

struct Sprite {
    uint8_t *data;
    int width;
    int size;
    Sprite(): data(nullptr), width(0), size(0) {}
    Sprite(uint8_t *data, uint8_t width, int size):
        data(data), width(width), size(size) {}
};

struct SpriteSlot {
    uint8_t *data; // Points to a space in one of the spriteXXX_memory banks where the sprite is loaded
    uint8_t sizeClass; // 64, 128, or 240
    int capacity; // sizeClass squared (the full size of the image)
    bool loaded;
    const char *filename;
    uint8_t ref_count;
    uint32_t last_access_us;
    Sprite sprite;
};
struct SpritePool {
    SpriteSlot slots240[SPRITE_LIMIT_240];
    SpriteSlot slots128[SPRITE_LIMIT_128];
    SpriteSlot slots64[SPRITE_LIMIT_64];
};

int mount_sd();
Sprite* load_sprite(const char *filename);
void release_sprite(Sprite *sprite);

#endif