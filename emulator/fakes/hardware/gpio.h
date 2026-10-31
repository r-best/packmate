#pragma once

#include "pico.h"

enum gpio_function {
    GPIO_FUNC_PWM     = 4,
    GPIO_FUNC_XIP_CS1 = 9,
};

#define GPIO_OUT 1

static inline void gpio_init(uint gpio) {}
static inline void gpio_set_dir(uint gpio, bool out) {}
static inline void gpio_put(uint gpio, bool value) {}
static inline void gpio_set_function(uint gpio, enum gpio_function fn) {}
