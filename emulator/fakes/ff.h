#pragma once

#include <stdint.h>

typedef unsigned int UINT;

typedef enum {
    FR_OK = 0,
    FR_DISK_ERR = 1,
    FR_NO_FILE = 4,
} FRESULT;

#define FA_READ 0x01

typedef struct {} FATFS;

typedef struct {
    void *handle;  // File opened under emulator/sdcard/
    uint64_t size;
} FIL;

#define f_size(fp) ((fp)->size)

FRESULT f_mount(FATFS *fs, const char *path, uint8_t opt);
FRESULT f_open(FIL *fp, const char *path, uint8_t mode);
FRESULT f_read(FIL *fp, void *buff, UINT btr, UINT *br);
FRESULT f_close(FIL *fp);
