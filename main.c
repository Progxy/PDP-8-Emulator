#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "utils.h"

bool isAValidFile(char* file) {
    char* fileExt = strchr(file, '.');
    if (fileExt == NULL) {
        return false;
    }
    
    if (!strcmp(fileExt, ".a")) {
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Error: you must include an assembly file!");
        return -1;
    } else if (!isAValidFile(argv[1])) {
        return -1;
    }

    char* filePath = argv[1];

    char* infoCompile = compileFile(filePath);

    if (strcmp(infoCompile, "OK")) {
        return -1;
    }

    runProgram();

    return 0;
}