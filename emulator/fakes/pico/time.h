#pragma once

#include "pico.h"

// Used in actual code
uint32_t time_us_32(void);
void sleep_ms(uint32_t ms);

// Transitively required by pimoroni libraries
typedef uint64_t absolute_time_t;
absolute_time_t get_absolute_time(void);
static inline uint32_t to_ms_since_boot(absolute_time_t t) { return (uint32_t)(t / 1000); }
