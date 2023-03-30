#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static word ram[4096];
static word* lcTable;
static word lc = 0x00;
static int lcIndex = 0;
static int linesCount = 0;

/// @brief Check if the given string is a valid ISA.
/// @param temp 
/// @param len 
/// @param file
/// @return Return true if the string contains the END pseudo-instruction, otherwise return false.
static bool isISA(char* temp, int len) {
    int index = 0;
    // Check if is a label
    if (contains(temp, ',')) {
        lcTable[lcIndex] = (temp[0] << 8) | temp[1];
        lcTable[lcIndex + 1] = (temp[2] << 8) | temp[3];
        lcTable[lcIndex + 2] = lc;
        lcIndex += 3;
        lc++;
        return false;
    } else if ((index = startsWith(temp, "ORG"))) {
        char hexVal[4];
        int j = 0;

        // Read the hex value
        for (int i = (index + 1); isAHexValue(temp[i]) && (j < 4); i++) {
            hexVal[j] = temp[i];
            j++;
        }

        lc = strToHex(hexVal, j);
        return false;
    } else if (startsWith(temp, "END")) {
        return true;
    }
    
    lc++;

    return false;
}

static bool compareLabels(int label, char* str, int len) {
    for (int i = 0; i < 4; i++) {
        if (str[i] != ((label >> (24 - (i * 8))) && (0b11111111))) {
            return false;
        }
    }
    return true;
}

static word resolveLabel(char* str, int len) {
    int val = 0;

    for (int i = 0; i < lcIndex; i+=3) {
        int label = (lcTable[i] << 16) | lcTable[i + 1];
        if (compareLabels(label, str, len)) {
            val = lcTable[i + 2];
            break;
        }
    }

    return val;
}

static bool isMRI(char* temp, int len) {
    int index = 0;
    word result = 0;

    /// add, lda, sta, bun, bsa, isz

    if ((index = startsWith(temp, "AND"))) {
        char label[7];
        int j = 0;

        for (int i = index + 1; temp[i] != '\0'; i++) {
            label[i] = temp[i];
            j++;
        }
        
        result = resolveLabel(label, j);
    }
}

static bool isInstruction(char* temp, int len) {
    int index = 0;
    
    // Check if is a pseudo-instruction
    if ((index = startsWith(temp, "ORG"))) {
        char hexVal[7];
        int j = 0;

        // Read the hex value
        for (int i = (index + 1); isAHexValue(temp[i]) && (j < 4); i++) {
            hexVal[j] = temp[i];
            j++;
        }

        lc = strToHex(hexVal, j);
        return false;
    }
    
    if (startsWith(temp, "END")) {
        return true;
    }

    if ((index = startsWith(temp, "DEC"))) {
        char val[7];
        int j = 0;

        for (int i = index + 1; temp[i] != '\0'; i++) {
            val[j] = temp[i];
            j++;
        }

        ram[lc] = parseInt(val, j);
        lc++;
        return false;
    }

    if ((index = startsWith(temp, "HEX"))) {
        char hex[7];
        int j = 0;

        for (int i = index + 1; temp[i] != '\0'; i++) {
            hex[j] = temp[i];
            j++;
        }

        ram[lc] = strToHex(hex, j);
        lc++;
        return false;
    }
    
    if (isMRI(temp, len)) {
        lc++;
        return false;
    }

    return false;
}

/// @brief Read the given file.
/// @param filePath 
/// @return Return the data contained in the file, but if occurs an error return NULL.
static char** readFile(char* filePath) {
    FILE* file = fopen(filePath, "r");
    char** data = (char**) malloc(sizeof(char*));
    char* str = (char*) malloc(sizeof(char));
    int charIndex = 0, strIndex = 0;

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

/// @brief Resolve the symbols inside the program.
/// @param data 
static void resolveSymbols(char** data) {
    // Implement the algorithm described in the first step of the assembler
    lc = 0;
    
    for (int i = 0; i < linesCount; i++) {
        if (compareStrings(data[i], "")) {
            continue;
        }

        if (isISA(data[i], strlen(data[i]))) {
            return;
        }
    }

    return;
}

/// @brief Assemble the program using the tables.
/// @param data 
static void assembleProgram(char** data) {
    lc = 0;
    
    for (int i = 0; i < linesCount; i++) {
        if (compareStrings(data[i], "")) {
            continue;
        }

        if (isInstruction(data[i], strlen(data[i]))) {
            return;
        }

    }
    return;
}

char* compileFile(char* filePath) {
    char** data = readFile(filePath);
    
    // Check for errors while opening the file
    if (data == NULL) {
        printf("Error: the file can't be opened!\n");
        return "ERR";
    }

    // First step of the assembler
    resolveSymbols(data);

    assembleProgram(data);

    printf("\nThe file has been compiled successfully!");

    return "OK";
}

void runProgram() {
    return;
}