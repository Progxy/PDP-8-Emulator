#pragma once

#ifndef ISA_H
#define ISA_H

#include "utils.h"

/// @brief Check if the given string is a valid ISA.
/// @param temp 
/// @return Return true if the string contains the END pseudo-instruction, otherwise return false.
bool isISA(char* str);

/// @brief Check if the given string is a MRI instruction, and if it does save the instruction in the ram.
/// @param str 
/// @return Return true if the given string is a MRI instruction, otherwise return false.
bool isMRI(char* str);

/// @brief Check if the given string is a RRI instruction, and if it does save the instruction in the ram.
/// @param str 
/// @return Return true if the given string is a RRI instruction, otherwise return false.
bool isRRI(char* str);

/// @brief Check if the given string is a IO instruction, and if it does save the instruction in the ram.
/// @param str 
/// @return Return true if the given string is a IO instruction, otherwise return false.
bool isIO(char* str);

/// @brief Check if the given string is a Pseudo-Instruction, and it it does save the content in the ram.
/// @param str 
/// @return Return true if the given string is a Pseudo-Instruction, otherwise return false.
bool isPseudoInstruction(char* str);

void andInstruction();

void addInstruction();

void ldaInstruction();

void staInstruction();

void bunInstruction();

void bsaInstruction();

void iszInstruction();

void claInstruction();

void cleInstruction();

void cmaInstruction();

void cmeInstruction();

void cirInstruction();

void cilInstruction();

void incInstruction();

void spaInstruction();

void snaInstruction();

void szaInstruction();

void szeInstruction();

void hltInstruction();

void inpInstruction();

void outInstruction();

#endif
