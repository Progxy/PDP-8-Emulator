#pragma once

#ifndef EMULATOR_H
#define EMULATOR_H
#endif

#include "utils.h"

word ac = 0;
word mbr = 0;
byte opr = 0;
byte e = 0;
byte i = 0;
byte s = 0;

/// @brief Emulate the program currently loaded in the ram.
/// @param stepMode
void emulate(bool step);

/// @brief Set on or off the machine.
void toggleMachineState();