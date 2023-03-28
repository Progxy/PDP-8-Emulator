#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static word ram[4096];
static word* lcTable;
static word lc = 0x00;
static int lcIndex = 0;

/// @brief Check if the given string is a valid ISA.
/// @param temp 
/// @param len 
/// @param file
/// @return Return true if the string is valid, otherwise return false.
static bool isISA(char* temp, int len, FILE* file) {
    // Check if is a label
    if (contains(temp, ',')) {
        lcTable[lcIndex] = (temp[0] << 8) | temp[1];
        lcTable[lcIndex + 1] = (temp[2] << 8) | temp[3];
        lcTable[lcIndex + 2] = lc;
        lcIndex += 3;
        return true;
    } else if (compareStrings("ORG", temp)) {
        // Read the space char
        char tmp = fgetc(file);
        char hexVal[3];

        // Read the hex value
        for (int i = 0; i < 4; i++) {
            tmp = fgetc(file);
            if (tmp == '\n') {
                break;
            }
            hexVal[i] = tmp;
        }

        lc = strToHex(hexVal);
        return true;
    } else if (compareStrings("    ", temp)) {
        lc++;
        return true;
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

        // Check if the string is ended
        if (tmp == '\n') {
            data[strIndex] = str;
            strIndex++;
            data = (char**) realloc(data, sizeof(char*) * (strIndex + 1));
            charIndex = 0;
            str = (char*) realloc(str, sizeof(char));
        }

        // Insert the char in the string
        str[charIndex] = tmp;
        charIndex++;
        str = (char*) realloc(str, sizeof(char) * (charIndex + 1));
    }
    // Read the part between the last newline and the EOF
    data[strIndex] = str;
    
    // Dispose the resources
    fclose(file);
    free(str);

    return data;
}

static void resolveSymbols(char** data) {
    // Implement the algorithm described in the first step of the assembler
}

char* compileFile(char* filePath) {
    char** data = readFile(filePath);
    
    // Check for errors while opening the file
    if (data == NULL) {
        printf("Error: the file can't be opened!\n");
        return "ERR";
    }

    resolveSymbols(data);

    printf("\nThe file has been compiled successfully!");

    return "OK";
}

void runProgram() {
    return;
}