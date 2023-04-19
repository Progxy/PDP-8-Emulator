#pragma once

#include "utils.h"

/// @brief Initialize the emulator by loading the flags and other info.
void initEmulator(bool flags);

/// @brief Emulate the program currently loaded in the ram.
void emulate();

/// @brief Set on or off the machine.
void toggleMachineState();

