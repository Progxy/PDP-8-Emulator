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
/// @return Return true if the string contains the END pseudo-instruction, otherwise return false.
static bool isISA(char* str) {
    int index = 0;
    // Check if is a label
    if (contains(str, ',')) {
        lcTable[lcIndex] = (str[0] << 8) | str[1];
        lcTable[lcIndex + 1] = (str[2] << 8) | str[3];
        lcTable[lcIndex + 2] = lc;
        lcIndex += 3;
        lc++;
        return false;
    } else if ((index = startsWith(str, "ORG"))) {
        char hexVal[4];
        int j = 0;

        // Read the hex value
        for (int i = (index + 1); isAHexValue(str[i]) && (j < 4); i++) {
            hexVal[j] = str[i];
            j++;
        }

        lc = strToHex(hexVal, j);
        return false;
    } else if (startsWith(str, "END")) {
        return true;
    }
    
    lc++;

    return false;
}

static bool compareLabels(int label, char* str) {
    for (int i = 0; i < 4; i++) {
        if (str[i] != ((label >> (24 - (i * 8))) && (0b11111111))) {
            return false;
        }
    }
    return true;
}

static void formatLabel(char* str, int len) {
    str[len] = ',';
    for (int i = len + 1; len < 4; i++) {
        str[i] = ' ';
    }
    return;
}

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

static bool isMRI(char* str) {
    int index = 0;
    word instruction = 0;

    if ((index = startsWith(str, "AND"))) {
        // Is not necessary to set the OPR, cause and OPR is 000
    } else if ((index = startsWith(str, "ADD"))) {
        // Set the OPR to 001
        instruction |= (0b001 << 12);
    } else if ((index = startsWith(str, "LDA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "STA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "BUN"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "BSA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "ISZ"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else {
        return false;
    }
    
    // Resolve the label once you've known the label position
    instruction = resolveLabel(str, index);
    
    // Save the instruction into the RAM and increment the lc
    ram[lc] = instruction;
    lc++;

    return true;
}

static bool isRRI(char* str) {
    if (containsWord(str, "CLA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111100000000000;
    } else if (containsWord(str, "CLE")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111010000000000;
    } else if (containsWord(str, "CMA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111001000000000;
    } else if (containsWord(str, "CME")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000100000000;
    } else if (containsWord(str, "CIR")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000010000000;
    } else if (containsWord(str, "CIL")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000001000000;
    } else if (containsWord(str, "INC")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000100000;
    } else if (containsWord(str, "SPA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000010000;
    } else if (containsWord(str, "SNA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000001000;
    } else if (containsWord(str, "SZA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000100;
    } else if (containsWord(str, "SZE")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000010;
    } else if (containsWord(str, "HLT")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000001;
    } else {
        return false;
    }

    // Increment the lc
    lc++;

    return true;
}

static bool isIO(char* str) {
    if (containsWord(str, "INP")) {
        // Set the instruction into the ram
        ram[lc] = 0b1111100000000000;
    } else if (containsWord(str, "OUT")) {
        // Set the instruction into the ram
        ram[lc] = 0b1111010000000000;
    } else {
        return false;
    }

    // Increment the lc
    lc++;
    return true;
}

static bool isInstruction(char* temp) {
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
    
    if (isMRI(temp)) {
        return false;
    }

    if (isRRI(temp)) {
        return false;
    }

    if (isIO(temp)) {
        return false;
    }
    
    // Increment the lc also if the string is invalid
    printf("\nWarning: the instruction at line %d is invalid!", lc);
    lc++;

    return false;
}

/// @brief Read the given file.
/// @param filePath 
/// @return Return the data contained in the file, but if occurs an error return NULL.
static char** readFile(char* filePath) {
    FILE* file = fopen(filePath, "r");
    char** data = (char**) malloc(sizeof(char*));
    char* str = (char*) calloc(1, sizeof(char));
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

/// @brief Resolve the symbols inside the program.
/// @param data 
static void resolveSymbols(char** data) {
    // Implement the algorithm described in the first step of the assembler
    lc = 0;
    
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

/// @brief Assemble the program using the tables.
/// @param data 
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