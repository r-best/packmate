#pragma once

#include "pico.h"
#include "hardware/structs/watchdog.h"

watchdog_hw_t *emulator_watchdog_hw(void);
#define watchdog_hw (emulator_watchdog_hw())

void watchdog_enable(uint32_t delay_ms, bool pause_on_debug);
void watchdog_update(void);
bool watchdog_caused_reboot(void);
