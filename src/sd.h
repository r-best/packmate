#include <stdio.h>
#include "pico/stdlib.h"

#ifndef SD_H
#define SD_H

#define SPRITE_LIMIT_240 2
#define SPRITE_LIMIT_128 10
#define SPRITE_LIMIT_64  10

#define SPRITE_SIZE_240 (240 * 240)
#define SPRITE_SIZE_128 (128 * 128)
#define SPRITE_SIZE_64  (64 * 64)

// Memory backing for sprites to be loaded into dynamically
static uint8_t sprite240_memory[SPRITE_LIMIT_240][SPRITE_SIZE_240];
static uint8_t sprite128_memory[SPRITE_LIMIT_128][SPRITE_SIZE_128];
static uint8_t sprite64_memory[SPRITE_LIMIT_64][SPRITE_SIZE_64];

struct SpriteSlot {
    uint8_t *data; // Points to a space in one of the spriteXXX_memory banks where the sprite is loaded
    uint8_t sizeClass; // 64, 128, or 240
    int capacity; // sizeClass squared (the full size of the image)
    bool in_use;
};
struct SpritePool {
    SpriteSlot slots240[SPRITE_LIMIT_240];
    SpriteSlot slots128[SPRITE_LIMIT_128];
    SpriteSlot slots64[SPRITE_LIMIT_64];
};
struct Sprite {
    uint8_t *data;
    int width;
    int size;
    Sprite(uint8_t *data, uint8_t width, int size):
        data(data), width(width), size(size) {}
};

int mount_sd();
Sprite* load_sprite(const char *filename);

#endif