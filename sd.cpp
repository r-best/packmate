#include <stdio.h>
#include "pico/stdlib.h"
#include "ff.h"
#include "f_util.h"

// Returns number of bytes read, or -1 on error
int read_file(const char *filename, char *out_buffer, size_t buffer_size) {
    FATFS fs;
    FIL fil;
    FRESULT fr;

    // Mount filesystem
    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card filesystem\n");
        return -1;
    }

    // Open file
    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK) {
        printf("Failed to open file: ");
        printf(filename);
        printf("\n");
        return -1;
    }

    UINT bytes_read;
    fr = f_read(&fil, out_buffer, buffer_size - 1, &bytes_read);
    if (fr != FR_OK) {
        printf("Failed to read file: ");
        printf(filename);
        printf("\n");
        f_close(&fil);
        return -1;
    }

    out_buffer[bytes_read] = '\0';  // null terminate

    f_close(&fil);

    return (int)bytes_read;
}