#include <stdio.h>
#include <string.h>
#include "lib.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error: you must include an assembly file!");
        return;
    }

    char* filePath = argv[1];

    char* infoCompile = compileFile(filePath);

    if (strcmp(infoCompile, "OK")) {
        return;
    }

    runProgram();

    return 0;
}