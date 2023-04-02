#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"

bool isAValidFile(char* file) {
    char* fileExt = strchr(file, '.');
    if (fileExt == NULL) {
        return false;
    }
    
    if (!strcmp(fileExt, ".s")) {
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error: you must include an assembly file (.s)!");
        return -1;
    } else if (!isAValidFile(argv[1])) {
        printf("\nError: invalid type of file!");
        return -1;
    }

    char* filePath = argv[1];

    char* infoCompile = compileFile(filePath);

    if (strcmp(infoCompile, "OK")) {
        return -1;
    }

    bool step = false;

    if (argc == 3) {
        step = containsWord(argv[2], "true");
    }

    // Turn on the machine
    toggleMachineState();

    // Start emulating the program loaded into the RAM
    emulate(step);

    toggleMachineState();

    return 0;
}