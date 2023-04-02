#pragma once

#include "utils.h"

inline word ac = 0;
inline word mbr = 0;
inline byte opr = 0;
inline byte e = 0;
inline byte i = 0;
inline byte s = 0;

/// @brief Emulate the program currently loaded in the ram.
/// @param stepMode
void emulate(bool step);

/// @brief Set on or off the machine.
void toggleMachineState();

