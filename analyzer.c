#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"

extern word pc;
extern word ram[4096];
char* mriInstructions[] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ", ""};
char* rriInstructions[] = {"HLT", "SZE", "SZA", "SNA", "SPA", "INC", "CIL", "CIR", "CME", "CMA", "CLE", "CLA", ""};    
char* ioInstructions[] = {"OUT", "INP", ""};

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
    int opr = getOPR(instruction); 

    // If the opr value is less than 7 is an MRI instruction
    if (opr < 7) {
        return mriInstructions[opr];
    }

    // Get the position of the 1 inside the address
    int index = logaritm(getAddress(instruction));
    
    return getI(instruction) ? ioInstructions[index - 10] : rriInstructions[index]; 
}

static bool getUserInput() {
    char quit;

    printf("\nPress ENTER to continue, or insert Q to quit: ");
    scanf("%c", &quit);

    if (quit != '\n') {
        cleanStdin();
    }

    // Delete the current line
    printf("\033[A\r");
    printf("\33[2K\r");

    return (quit == 'q' || quit == 'Q');
}

/// @brief Print the info of the instruction at the given index.
/// @param index 
static void printInstructionInfo(word index) {
    printHex(index, 4);
    printf(":  ");
    printBits(ram[index], getBitSize(ram[index]));
    printf("    %d      %d      %d -", getI(ram[index]), getOPR(ram[index]), getAddress(ram[index]));
    printHex(getAddress(ram[index]), 4);
    printf("     %s", getInstruction(ram[index]));
    return;
}

void analyzeInstructions() {
    printf("\n---------    INSTRUCTION    --- I --- OPR ---   ADDRESS    --- ISA ---------\n");
    
    // Init the counter value with the value of the program counter
    word counter = pc;

    // Print the instructions until the user wants to exit
    while (1) {
        if (!ram[counter] && !ram[(counter + 1) % 4096]) {
            // Print the current instruction that is empty
            printInstructionInfo(counter);
            
            // Get the number of empty instructions
            int emptyInstructionsCounter = 0;
            while (!ram[counter]) {
                counter++;
                counter = counter % 4096;
                emptyInstructionsCounter++;
            }

            printf("\n *\n *\n *\n Other %d lines of empty instructions\n *\n *\n *\n", emptyInstructionsCounter);
        }

        printInstructionInfo(counter);

        if (getUserInput()) {
            break;
        }

        // Map the counter value to prevent access at invalid index
        counter++;
        counter = counter % 4096;
    }

    printf("----------------------------------------------------------------------------\n");
    return;
}

void dumpInstructions(bool dumpOut) {
    return;
}