#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"
#include "analyzer.h"

int main(int argc, char** argv) {
    bool* flags = getFlags(argc, argv);
    if (!flags[0]) {
        printf("Error: you must include an assembly file (.s/.asm/.S/.pdp8)!");
        return -1;
    } else if (flags[1]) {
        // Check for the help flag
        printHelpInfo();
        return 0;
    } 
    
    // Check if the given file extension is valid
    if (!isAValidFile(argv[1])) {
        printf("\nError: invalid type of file, you must include an assembly file (.s, .asm, .S, .pdp8)!");
        return -1;
    }

    char* filePath = argv[1];

    // Check the status of the compilation
    if (compileFile(filePath)) {
        return -1;
    }

    if (flags[2]) {
        // Analyze the instructions after the compilation of the given file
        analyzeInstructions();
        return 0;
    } else if (flags[3]) {
        // Dump the instructions compiled inside a txt file, pass the DUMP_OUT flag to check if the user gave a filename for the output file
        dumpInstructions(flags[4]);
        return 0;
    }

    // Init emulator
    initEmulator(flags[5]);

    // Start emulating the program loaded into the RAM
    emulate();

    // Remove the flags array from the heap
    free(flags);

    return 0;
}