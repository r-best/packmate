#pragma once

#include "pico.h"

#define XIP_CTRL_WRITABLE_M1_BITS 0x00000800u // Value copied from pico lib

typedef struct {
    volatile uint32_t ctrl;
} xip_ctrl_hw_t;

inline xip_ctrl_hw_t xip_ctrl_regs;
#define xip_ctrl_hw (&xip_ctrl_regs)

static inline void hw_set_bits(volatile uint32_t *addr, uint32_t mask) { *addr |= mask; }
