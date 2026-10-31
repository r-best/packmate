#pragma once

#include "pico.h"

typedef struct spi_inst spi_inst_t; // Fake spi struct pretending to be the actual struct from pimoroni lib
#define spi1 ((spi_inst_t *)(uintptr_t)0x67) // six sevennnnnnnn (spi1 pointer mocked out as some arbitrary number)

static inline uint spi_init(spi_inst_t *spi, uint baudrate) { return baudrate; }
static inline int spi_write_blocking(spi_inst_t *spi, const uint8_t *src, size_t len) { return (int)len; }
