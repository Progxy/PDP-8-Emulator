#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"
#include "analyzer.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error: you must include an assembly file (.s)!");
        return -1;
    } else if (!isAValidFile(argv[1])) {
        printf("\nError: invalid type of file, you must include an assembly file (.s, .asm, .S, .pdp8)!");
        return -1;
    }

    char* filePath = argv[1];

    // Check the status of the compilation
    if (compileFile(filePath)) {
        return -1;
    }

    bool flag = false;

    // Check for extra flags
    if (argc == 3) {
        // Check the compile only flag
        if (!strcmp(argv[2], "-c")) {
            // Let the user analyze the compiled program
            analyzeInstructions();
            return 0;
        }
    
        flag = !strcmp(argv[2], "-step");
    }

    // Init emulator
    initEmulator(flag);

    // Start emulating the program loaded into the RAM
    emulate();

    return 0;
}