#pragma once

#include "pico.h"
#include "hardware/gpio.h"

static inline uint pwm_gpio_to_slice_num(uint gpio) { return (gpio >> 1u) & 7u; }
static inline uint pwm_gpio_to_channel(uint gpio) { return gpio & 1u; }

void pwm_set_enabled(uint slice_num, bool enabled);
void pwm_set_clkdiv(uint slice_num, float divider);
void pwm_set_wrap(uint slice_num, uint16_t wrap);
void pwm_set_chan_level(uint slice_num, uint chan, uint16_t level);
