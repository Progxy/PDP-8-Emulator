#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"
#include "isa.h"

word ram[4096];
word* lcTable;
word lc;
int lcIndex;
static int linesCount;
word pc;
bool errorFlag = false;

/* DECLARATION OF THE INTERNAL FUNCTIONS */

/// @brief Check if the given string is an instruction, and if it does save the instruction in the ram.
/// @param temp 
/// @param currentLine
/// @return Return true if the given string contains the pseudo-instruction 'END', otherwise return false.
static bool isInstruction(char* str, int currentLine);

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

    if (errorFlag) {
        printf("\nThe compilation terminated with an error!");
        return "FAILED";
    }

    printf("\nThe file has been compiled successfully!");

    return "OK";
}

/* DEFINITION OF THE INTERNAL FUNCTIONS */

static bool isInstruction(char* str, int currentLine) {
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
        if (errorFlag) {
            printf("\nError: Unresolved symbol at line %d!", currentLine);
        }
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
    printf("\nWarning: the instruction at line %d is invalid!", currentLine + 1);
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
        if ((tmp == '/') && (!charIndex)) {
            // Read till the end of the line
            while (fgetc(file) != '\n');
            charIndex = 0;
            str = (char*) calloc(1, sizeof(char));
            linesCount++;
            continue;
        } else if (tmp == '/') {
            // Read till the end of the line
            while (fgetc(file) != '\n');
            str[charIndex] = '\0';
            data[strIndex] = str;
            strIndex++;
            data = (char**) realloc(data, sizeof(char*) * (strIndex + 1));
            charIndex = 0;
            str = (char*) calloc(1, sizeof(char));
            linesCount++;
            continue;
        }

        // Check if the string is ended
        if (tmp == '\n') {
            str[charIndex] = '\0';
            data[strIndex] = str;
            strIndex++;
            data = (char**) realloc(data, sizeof(char*) * (strIndex + 1));
            charIndex = 0;
            str = (char*) calloc(1, sizeof(char));
            continue;
        }

        // Insert the char in the string
        str[charIndex] = tmp;
        charIndex++;
        str = (char*) realloc(str, sizeof(char) * (charIndex + 1));
    }

    // Read the part between the last newline and the EOF
    data[strIndex] = str;
    linesCount += strIndex + 1;
    
    // Dispose the resources
    fclose(file);

    return data;
}

static void resolveSymbols(char** data) {
    lc = 0;
    lcIndex = 0;
    lcTable = (word*) calloc(3, sizeof(word));

    // Init pc to -1, to check if the pc has been already loaded
    pc = -1;
    
    for (int i = 0; i < linesCount; i++) {
        if (!strlen(data[i])) {
            printf("\nThe string at line: %d is empty!", i + 1);
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
        if (!strlen(data[i])) {
            printf("\nThe string at line: %d is empty!", i + 1);
            continue;
        }

        if (isInstruction(data[i], i) || errorFlag) {
            return;
        }

    }

    return;
}

/* END OF DEFINITION */
