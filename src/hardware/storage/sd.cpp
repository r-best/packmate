#include "sd.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
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

    struct SpriteSlot {
        uint8_t *data; // Points to a space in one of the sprite memory banks over in sd.cpp
        bool loaded;
        char filename[64];
        uint8_t ref_count;
        uint32_t last_access_us;
        Sprite sprite;
    };

    struct SpriteSubPool {
        SpriteSlot *slots;
        int count;
        int capacity;
    };
    struct SpritePool {
        SpriteSubPool pools[3];
        SpriteSlot slots64[SPRITE_LIMIT_64];
        SpriteSlot slots128[SPRITE_LIMIT_128];
        SpriteSlot slots240[SPRITE_LIMIT_240];
        SpritePool() {
            pools[0] = { slots64,  SPRITE_LIMIT_64,  SPRITE_SIZE_64  };
            pools[1] = { slots128, SPRITE_LIMIT_128, SPRITE_SIZE_128 };
            pools[2] = { slots240, SPRITE_LIMIT_240, SPRITE_SIZE_240 };
        }
    };

    static FATFS fs;
    static SpritePool pool;

    static void init_sprite_slot(SpriteSlot &slot, uint8_t *data) {
        slot.data = data;
        slot.loaded = false;
        slot.filename[0] = '\0';
        slot.ref_count = 0;
        slot.last_access_us = 0;
        slot.sprite = Sprite();
    }

    int init() {
        // Mount the SD card
        FRESULT fr = f_mount(&fs, "", 1);
        if (fr != FR_OK) {
            printf("Failed to mount SD card filesystem\n");
            return -1;
        }

        // Initialize all SpriteSlots in pool, sprites will be loaded into these later
        for(int i = 0; i < SPRITE_LIMIT_240; i++)
            init_sprite_slot(pool.slots240[i], sprite240_memory[i]);
        for(int i = 0; i < SPRITE_LIMIT_128; i++)
            init_sprite_slot(pool.slots128[i], sprite128_memory[i]);
        for(int i = 0; i < SPRITE_LIMIT_64; i++)
            init_sprite_slot(pool.slots64[i], sprite64_memory[i]);

        return 0;
    }

    static SpriteSlot* check_cache(const char *filename) {
        for (SpriteSubPool &p : pool.pools) {
            for (int i = 0; i < p.count; i++) {
                if (p.slots[i].loaded && strcmp(p.slots[i].filename, filename) == 0)
                    return &p.slots[i];
            }
        }
        return nullptr;
    }

    static SpriteSlot* find_slot_in_pool(SpriteSlot *slots, int count) {
        SpriteSlot *candidate = nullptr;
        uint32_t oldest = UINT32_MAX;
        for (int i = 0; i < count; i++) {
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
        }
        return candidate;
    }

    static SpriteSlot* find_best_pool(int needed_bytes) {
        for (SpriteSubPool &p : pool.pools) {
            if (needed_bytes > p.capacity) continue;
            SpriteSlot *slot = find_slot_in_pool(p.slots, p.count);
            if (slot) return slot;
        }
        printf("Error loading sprite - no free or evictable slot for %d bytes\n", needed_bytes);
        return nullptr;
    }

    Sprite* load_sprite(const char *filename, uint8_t frame_width, uint8_t frame_count) {
        SpriteSlot *cached = check_cache(filename);
        if (cached) {
            cached->ref_count++;
            cached->last_access_us = time_us_32();
            return &cached->sprite;
        }

        FIL fil;
        FRESULT fr = f_open(&fil, filename, FA_READ);
        if (fr != FR_OK) {
            printf("Failed to open file: %s\n", filename);
            return nullptr;
        }
        int file_size = f_size(&fil);

        int needed_bytes = frame_width * frame_width * frame_count;
        SpriteSlot *slot = find_best_pool(needed_bytes);
        if (!slot) {
            f_close(&fil);
            return nullptr;
        }

        UINT bytes_read;
        fr = f_read(&fil, slot->data, needed_bytes, &bytes_read);
        if (fr != FR_OK || (int)bytes_read != needed_bytes) {
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
        slot->sprite.width = frame_width;
        slot->sprite.frame_count = frame_count;
        slot->sprite.size = needed_bytes;
        return &slot->sprite;
    }

    void release_sprite(Sprite *sprite) {
        for (SpriteSubPool &p : pool.pools) {
            for (int i = 0; i < p.count; i++) {
                if (&p.slots[i].sprite == sprite) {
                    if (p.slots[i].ref_count > 0) p.slots[i].ref_count--;
                    return;
                }
            }
        }
        printf("Warning: release_sprite called with unknown sprite\n");
    }
}
