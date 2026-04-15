#include "sd.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "ff.h"
#include "f_util.h"

// Memory backing for sprites to be loaded into dynamically
static uint8_t sprite240_memory[SPRITE_LIMIT_240][SPRITE_SIZE_240];
static uint8_t sprite128_memory[SPRITE_LIMIT_128][SPRITE_SIZE_128];
static uint8_t sprite64_memory[SPRITE_LIMIT_64][SPRITE_SIZE_64];

static FATFS fs;
static SpritePool pool;

int mount_sd() {
    // Mount the SD card
    FRESULT fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card filesystem\n");
        return -1;
    }

    // Initialize all SpriteSlots in pool, sprites will be loaded into these later
    for(int i = 0; i < SPRITE_LIMIT_240; i++){
        pool.slots240[i] = { sprite240_memory[i], 240, SPRITE_SIZE_240, false };
    }
    for(int i = 0; i < SPRITE_LIMIT_128; i++){
        pool.slots128[i] = { sprite128_memory[i], 128, SPRITE_SIZE_128, false };
    }
    for(int i = 0; i < SPRITE_LIMIT_64; i++){
        pool.slots64[i] = { sprite64_memory[i], 64, SPRITE_SIZE_64, false };
    }
    return 0;
}

SpriteSlot* find_free_slot(int size) {
    SpriteSlot *slots = nullptr;
    int capacity = 0;
    switch (size) {
    case SPRITE_SIZE_240:
        slots = pool.slots240;
        capacity = SPRITE_LIMIT_240;
        break;
    case SPRITE_SIZE_128:
        slots = pool.slots128;
        capacity = SPRITE_LIMIT_128;
        break;
    case SPRITE_SIZE_64:
        slots = pool.slots64;
        capacity = SPRITE_LIMIT_64;
        break;
    default:
        break;
    }

    for(int i = 0; i < capacity; i++) {
        if(!slots[i].in_use){
            slots[i].in_use = true;
            return &slots[i];
        }
    }
    printf("Error loading sprite - no free slot\n");
    return nullptr;
}

// Returns number of bytes read, or -1 on error
Sprite* load_sprite(const char *filename) {
    FIL fil;
    FRESULT fr;

    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK) {
        printf("Failed to open file: %s\n", filename);
        return nullptr;
    }

    // Determine size of slot needed and find (or create) an open slot
    SpriteSlot *slot = find_free_slot(f_size(&fil));
    
    UINT bytes_read;
    fr = f_read(&fil, slot->data, slot->capacity, &bytes_read);
    if (fr != FR_OK) {
        printf("Failed to read file: %s\n", filename);
        f_close(&fil);
        return nullptr;
    }

    f_close(&fil);
    return new Sprite(slot->data, slot->sizeClass, slot->capacity);
}