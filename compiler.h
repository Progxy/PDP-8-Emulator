#pragma once

#include "utils.h"

inline word ram[4096];
inline word* lcTable;
inline word lc;
inline int lcIndex;
inline int linesCount;
inline word pc;

/// @brief Compile the given file.
/// @param filePath 
/// @return Return the compilation result.
char* compileFile(char* filePath);

