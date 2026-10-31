#pragma once

#include <stdint.h>

typedef struct {
    volatile uint32_t scratch[8];
} watchdog_hw_t;
