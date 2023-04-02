#pragma once

#ifndef COMPILER_H
#define COMPILER_H
#endif


#include "utils.h"

word ram[4096];
word* lcTable;
word lc;
int lcIndex;
int linesCount;
word pc;

/// @brief Compile the given file.
/// @param filePath 
/// @return Return the compilation result.
char* compileFile(char* filePath);
