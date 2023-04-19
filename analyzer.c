#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"

extern word pc;
extern word ram[4096];

/// @brief Get the I value inside the given instruction.
/// @param instruction 
/// @return Return the I value found inside the given instruction;
static byte getI(word instruction) {
    return ((instruction >> 15) & 0b01);
}

/// @brief Get the OPR code inside the given instruction.
/// @param instruction 
/// @return Return the OPR code found inside the given instruction;
static byte getOPR(word instruction) {
    return ((instruction >> 12) & 0b0111);
}

/// @brief Get the address inside the given instruction.
/// @param instruction 
/// @return Return the address found inside the given instruction;
static word getAddress(word instruction) {
    return (instruction & 0b111111111111);
}

static char* getInstruction(word instruction) {
    const char* mriInstructions[] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ"};
    const char* rriInstructions[] = {"HLT", "SZE", "SZA", "SNA", "SPA", "INC", "CIL", "CIR", "CME", "CMA", "CLE", "CLA"};    
    const char* ioInstructions[] = {"OUT", "INP"};

    byte opr = getOPR(instruction); 

    // If the opr value is less than 7 is an MRI instruction
    if (opr < 7) {
        return mriInstructions[opr];
    }

    // Get the position of the 1 inside the address
    int index = logaritm(getAddress(instruction));
    
    return getI(instruction) ? ioInstructions[index - 10] : rriInstructions[index]; 
}

static int getUserInput() {
    char hex[5];

    printf("\nPress ENTER to continue to the next address, or insert a valid hexadecimal value to use as next address: ");
    fgets(hex, 4, stdin);

    word hexAddress = strToHex(hex, strlen(hex));
    cleanStdin();

    // Delete the current line
    printf("\33[2K\r");

    return (hexAddress < 4096) ? hexAddress : -1;
}

void analyzeInstructions() {
    printf("\n--- I --- OPR --- ADDRESS ------------ INSTRUCTION ------\n");
    
    // Init the counter value with the value of the program counter
    int counter = pc;

    // Print the instructions until the user wants to exit
    while (counter == 0) {
        printHex(counter, 4);
        printf(": %d    %d    %d -", getI(ram[counter]), getOPR(ram[counter]), getAddress(ram[counter]));
        printHex(getAddress(ram[counter]), 4);
        printf("    %s\n", getInstruction(ram[counter]));

        // Ask the user to print the next instruction or to insert a valid hexadecimal address
        if ((counter = getUserInput()) == -1) {
            break;
        }

        // Map the counter value to prevent access at invalid index
        counter++;
        counter = counter % 4096;
    }

    printf("\n--------------------------------\n");
    return;
}