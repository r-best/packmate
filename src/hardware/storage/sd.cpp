#include "sd.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "ff.h"
#include "f_util.h"

namespace SD {
    // Direct pointers into the uncached PSRAM alias (0x15000000 = XIP_NOCACHE_NOALLOC + CS1 offset).
    // We want to skip the XIP cache because these aren't code instructions, they're large data blocks, they
    // would overwhelm the small cache and cause a bunch of misses when actual instructions try to run
    static constexpr uintptr_t PSRAM_BASE  = 0x15000000u;
    static constexpr size_t    POOL240_SZ  = (size_t)SPRITE_LIMIT_240 * SPRITE_SIZE_240;
    static constexpr size_t    POOL128_SZ  = (size_t)SPRITE_LIMIT_128 * SPRITE_SIZE_128;
    static uint8_t (* const sprite240_memory)[SPRITE_SIZE_240] = (uint8_t(*)[SPRITE_SIZE_240])(PSRAM_BASE);
    static uint8_t (* const sprite128_memory)[SPRITE_SIZE_128] = (uint8_t(*)[SPRITE_SIZE_128])(PSRAM_BASE + POOL240_SZ);
    static uint8_t (* const sprite64_memory)[SPRITE_SIZE_64] = (uint8_t(*)[SPRITE_SIZE_64])  (PSRAM_BASE + POOL240_SZ + POOL128_SZ);

    static FATFS fs;
    static SpritePool pool;

    static void init_sprite_slot(SpriteSlot &slot, uint8_t *data, uint8_t sizeClass, int capacity) {
        slot.data = data;
        slot.sizeClass = sizeClass;
        slot.capacity = capacity;
        slot.loaded = false;
        slot.filename[0] = '\0';
        slot.ref_count = 0;
        slot.last_access_us = 0;
        slot.sprite = Sprite(data, sizeClass, capacity);
    }

    int init() {
        // Mount the SD card
        FRESULT fr = f_mount(&fs, "", 1);
        if (fr != FR_OK) {
            printf("Failed to mount SD card filesystem\n");
            return -1;
        }

        // Initialize all SpriteSlots in pool, sprites will be loaded into these later
        for(int i = 0; i < SPRITE_LIMIT_240; i++){
            init_sprite_slot(pool.slots240[i], sprite240_memory[i], 240, SPRITE_SIZE_240);
        }
        for(int i = 0; i < SPRITE_LIMIT_128; i++){
            init_sprite_slot(pool.slots128[i], sprite128_memory[i], 128, SPRITE_SIZE_128);
        }
        for(int i = 0; i < SPRITE_LIMIT_64; i++){
            init_sprite_slot(pool.slots64[i], sprite64_memory[i], 64, SPRITE_SIZE_64);
        }
        return 0;
    }

    static SpriteSlot* check_cache(const char *filename) {
        for (int i = 0; i < SPRITE_LIMIT_240; i++) {
            if (pool.slots240[i].loaded && strcmp(pool.slots240[i].filename, filename) == 0) {
                return &pool.slots240[i];
            }
        }
        for (int i = 0; i < SPRITE_LIMIT_128; i++) {
            if (pool.slots128[i].loaded && strcmp(pool.slots128[i].filename, filename) == 0) {
                return &pool.slots128[i];
            }
        }
        for (int i = 0; i < SPRITE_LIMIT_64; i++) {
            if (pool.slots64[i].loaded && strcmp(pool.slots64[i].filename, filename) == 0) {
                return &pool.slots64[i];
            }
        }
        return nullptr;
    }

    static SpriteSlot* find_free_slot(int size) {
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

        if (!slots) {
            return nullptr;
        }

        SpriteSlot *candidate = nullptr;
        uint32_t oldest = UINT32_MAX;
        for (int i = 0; i < capacity; i++) {
            if (!slots[i].loaded) {
                slots[i].loaded = true;
                return &slots[i];
            }
            if (slots[i].ref_count == 0 && slots[i].last_access_us < oldest) {
                oldest = slots[i].last_access_us;
                candidate = &slots[i];
            }
        }

        if (candidate) {
            candidate->filename[0] = '\0';
            candidate->last_access_us = 0;
            return candidate;
        }

        printf("Error loading sprite - no free or evictable slot\n");
        return nullptr;
    }

    Sprite* load_sprite(const char *filename) {
        SpriteSlot *cached = check_cache(filename);
        if (cached) {
            cached->ref_count += 1;
            cached->last_access_us = time_us_32();
            return &cached->sprite;
        }

        FIL fil;
        FRESULT fr = f_open(&fil, filename, FA_READ);
        if (fr != FR_OK) {
            printf("Failed to open file: %s\n", filename);
            return nullptr;
        }

        int size = f_size(&fil);
        SpriteSlot *slot = find_free_slot(size);
        if (!slot) {
            f_close(&fil);
            return nullptr;
        }

        UINT bytes_read;
        fr = f_read(&fil, slot->data, slot->capacity, &bytes_read);
        if (fr != FR_OK || bytes_read != (UINT)slot->capacity) {
            printf("Failed to read file: %s\n", filename);
            f_close(&fil);
            slot->loaded = false;
            slot->filename[0] = '\0';
            slot->ref_count = 0;
            slot->last_access_us = 0;
            return nullptr;
        }

        f_close(&fil);

        strncpy(slot->filename, filename, sizeof(slot->filename) - 1);
        slot->filename[sizeof(slot->filename) - 1] = '\0';
        slot->ref_count = 1;
        slot->last_access_us = time_us_32();
        slot->sprite.data = slot->data;
        slot->sprite.width = slot->sizeClass;
        slot->sprite.size = slot->capacity;
        return &slot->sprite;
    }

    void release_sprite(Sprite *sprite) {
        for (int i = 0; i < SPRITE_LIMIT_240; i++) {
            if (&pool.slots240[i].sprite == sprite) {
                if (pool.slots240[i].ref_count > 0) {
                    pool.slots240[i].ref_count--;
                }
                return;
            }
        }
        for (int i = 0; i < SPRITE_LIMIT_128; i++) {
            if (&pool.slots128[i].sprite == sprite) {
                if (pool.slots128[i].ref_count > 0) {
                    pool.slots128[i].ref_count--;
                }
                return;
            }
        }
        for (int i = 0; i < SPRITE_LIMIT_64; i++) {
            if (&pool.slots64[i].sprite == sprite) {
                if (pool.slots64[i].ref_count > 0) {
                    pool.slots64[i].ref_count--;
                }
                return;
            }
        }
        printf("Warning: release_sprite called with unknown sprite\n");
    }
}
