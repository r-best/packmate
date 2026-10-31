#pragma once

#include "emulator/state.h"

extern EmulatorState *emulator;

// Thrown to unwind firmware threads on reload
struct EmulatorStop {};

// Throws EmulatorStop if the host has asked the firmware to stop
void emulator_check_stop();
