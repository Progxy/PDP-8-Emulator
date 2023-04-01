#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"
#include "isa.h"

static int tick = 0;
static int cycle = 0;

/* DECLARATION OF THE INTERNAL FUNCTIONS */

/// @brief Resolve the label that there's in the given string.
/// @param str 
/// @param index 
/// @return Return the address of the label.
static word resolveLabel(char* str, int index);

/// @brief Check if the given string is an instruction, and if it does save the instruction in the ram.
/// @param temp 
/// @return Return true if the given string contains the pseudo-instruction 'END', otherwise return false.
static bool isInstruction(char* str);

/// @brief Read the given file.
/// @param filePath 
/// @return Return the data contained in the file, but if occurs an error return NULL.
static char** readFile(char* filePath);

/// @brief Resolve the symbols inside the program.
/// @param data 
static void resolveSymbols(char** data);

/// @brief Assemble the program using the tables.
/// @param data 
static void assembleProgram(char** data);

/* END OF DECLARATION */

char* compileFile(char* filePath) {
    char** data = readFile(filePath);
    
    // Check for errors while opening the file
    if (data == NULL) {
        printf("Error: the file can't be opened!\n");
        return "ERR";
    }

    // First step of the assembler
    resolveSymbols(data);

    // Second step of the assembler (the real assembly is done here!)
    assembleProgram(data);

    printf("\nThe file has been compiled successfully!");

    return "OK";
}

void runProgram() {
    // Map the cycle counter between 0 and 3
    cycle = cycle % 4;

    // Switch between the four cycles
    switch (cycle) {
        case 0:
            fetchCycle();
            cycle++;
            break;
        
        case 1:
            IMACycle();
            cycle++;
            break;        
            
        case 2:
            executionCycle();
            cycle++;
            break;        
            
        case 3:
            interruptCycle();
            cycle++;
            break;
        
        default:
            break;
    }

    return runProgram();
}

/* DEFINITION OF THE INTERNAL FUNCTIONS */

static word resolveLabel(char* str, int index) {
    char label[5];
    int j = 0;

    for (int i = index + 1; (str[i] != '\0') && (j < 3) && (str[i] != ' '); i++) {
        label[j] = str[i];
        j++;
    }

    // Format the label to match the structure of the labels stored
    formatLabel(label, j);
    int val = 0;

    for (int i = 0; i < lcIndex; i+=3) {
        int storedLabel = (lcTable[i] << 16) | lcTable[i + 1];
        if (compareLabels(storedLabel, label)) {
            val = lcTable[i + 2];
            break;
        }
    }

    // Check if the instruction uses indirect memory addressing
    val |= (contains(str, 'I') << 15);

    return val;
}

static bool isInstruction(char* str) {
    int index = 0;
    
    // Check if is a pseudo-instruction
    if (isPseudoInstruction(str)) {
        return false;
    }
    
    // Check if contains the 'END' pseudo-instruction
    if (startsWith(str, "END")) {
        return true;
    }
    
    // Check if is a MRI instruction
    if (isMRI(str)) {
        return false;
    }

    // Check if is a RRI instruction
    if (isRRI(str)) {
        return false;
    }

    // Check if is a IO instruction
    if (isIO(str)) {
        return false;
    }
    
    // Increment the lc also if the string is invalid
    printf("\nWarning: the instruction at line %d is invalid!", lc);
    lc++;

    return false;
}

static char** readFile(char* filePath) {
    FILE* file = fopen(filePath, "r");
    char** data = (char**) malloc(sizeof(char*));
    char* str = (char*) calloc(1, sizeof(char));
    int charIndex = 0, strIndex = 0;
    linesCount = 0;

    // Check for errors while opening the file
    if (file == NULL) {
        return NULL;
    }

    // Read the input stream and generate the instructions 
    while (!feof(file)) {
        char tmp = fgetc(file);

        // Check if is a comment, and if uses all the line or is at the end of an instruction
        if ((tmp == '\\') && (!charIndex)) {
            // Read till the end of the line
            while (fgetc(file) != '\n');
            charIndex = 0;
            str = (char*) realloc(str, sizeof(char));
            str[0] = '\0';
            continue;
        } else if (tmp == '\\') {
            // Read till the end of the line
            while (fgetc(file) != '\n');
            str[charIndex] = '\0';
            data[strIndex] = str;
            strIndex++;
            data = (char**) realloc(data, sizeof(char*) * (strIndex + 1));
            charIndex = 0;
            str = (char*) realloc(str, sizeof(char));
            str[0] = '\0';
            continue;
        }

        // Check if the string is ended
        if (tmp == '\n') {
            str[charIndex] = '\0';
            data[strIndex] = str;
            strIndex++;
            data = (char**) realloc(data, sizeof(char*) * (strIndex + 1));
            charIndex = 0;
            str = (char*) realloc(str, sizeof(char));
            str[0] = '\0';
            continue;
        }

        // Insert the char in the string
        str[charIndex] = tmp;
        charIndex++;
        str = (char*) realloc(str, sizeof(char) * (charIndex + 1));
    }

    // Read the part between the last newline and the EOF
    data[strIndex] = str;
    linesCount = strIndex + 1;
    
    // Dispose the resources
    fclose(file);
    free(str);

    return data;
}

static void resolveSymbols(char** data) {
    lc = 0;
    lcIndex = 0;
    // Init pc to -1
    pc = -1;
    
    for (int i = 0; i < linesCount; i++) {
        if (compareStrings(data[i], "")) {
            continue;
        }

        if (isISA(data[i])) {
            return;
        }

    }

    return;
}

static void assembleProgram(char** data) {
    lc = 0;
    
    for (int i = 0; i < linesCount; i++) {
        if (compareStrings(data[i], "")) {
            continue;
        }

        if (isInstruction(data[i])) {
            return;
        }

    }

    return;
}

/* END OF DEFINITION */
