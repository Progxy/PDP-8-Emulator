#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"

bool stepFlag = false;

bool isAValidFile(char* file) {
    char* fileExt = strchr(file, '.');
    if (fileExt == NULL) {
        return false;
    }
    
    if (!(strcmp(fileExt, ".s") || strcmp(fileExt, ".asm") || strcmp(fileExt, ".S") || strcmp(fileExt, ".pdp8"))) {
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error: you must include an assembly file (.s)!");
        return -1;
    } else if (!isAValidFile(argv[1])) {
        printf("\nError: invalid type of file, you must include an assembly file (.s, .asm, .S, .pdp8)!");
        return -1;
    }

    char* filePath = argv[1];

    char* infoCompile = compileFile(filePath);

    // Check the status of the compilation
    if (strcmp(infoCompile, "OK")) {
        return -1;
    }

    // Check for extra flags
    if (argc == 3) {
        stepFlag = !strcmp(argv[2], "-step");
        // Check the compile only flag
        if (!strcmp(argv[2], "-c")) {
            return 0;
        }
    }

    // Turn on the machine
    toggleMachineState();

    printf("\n------- OUTPUT: -------\n");

    // Start emulating the program loaded into the RAM
    emulate();

    printf("\n-----------------------\n");

    return 0;
}