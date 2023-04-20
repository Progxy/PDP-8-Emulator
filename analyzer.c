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

/// @brief Collect all the data about the instruction inside a string.
/// @param index 
/// @return Return the string containing the instruction's info.
static char* getInstructionInfo(int index) {
    char* instructionInfo = (char*) calloc(4, sizeof(char));
    char* temp = NULL;
    int tempLen = 0;
    word instruction = ram[index];

    printf("\nHere: 1");

    // Copy the current memory address as an hex value
    char* instructionHex = convertToHex(index, 4);
    strcat(instructionInfo, instructionHex);
    free(instructionHex);
    
    printf("\nHere: 2");

    // Copy the string inside the instruction's info
    strcat(instructionInfo, ":  ");
    
    printf("\nHere: 3");

    // Copy the instruction as a binary string
    char* instructionBits = convertToBits(instruction, getBitSize(instruction));
    strcat(instructionInfo, instructionBits);
    free(instructionBits);
    
    printf("\nHere: 4");

    // Copy the value of the registers and the address 
    temp = (char*) calloc(125, sizeof(char));
    tempLen = sprintf(temp, "    %d      %d      %d -", getI(instruction), getOPR(instruction), getAddress(instruction));
    temp = (char*) realloc(temp, tempLen + 1);
    strcat(instructionInfo, temp);
    free(temp);
    
    printf("\nHere: 5");

    // Copy the value of the current address as an hex value
    char* addressHex = convertToHex(getAddress(instruction), 4);
    strcat(instructionInfo, addressHex);
    free(addressHex);
    
    printf("\nHere: 6");

    // Copy the name of the current instruction
    temp = (char*) calloc(25, sizeof(char));
    tempLen = sprintf(temp, "     %s", getInstruction(instruction));
    temp = (char*) realloc(temp, tempLen + 1);
    strcat(instructionInfo, temp);
    free(temp);
    
    return instructionInfo;
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

void dumpInstructions(bool dumpOut, char** argv) {
    // Create the output file
    FILE* outFile = NULL;

    if (dumpOut) {
        // Check if the given outfile name is valid
        char* fileExt = strchr(argv[3], '.');
        if ((fileExt == NULL) || strcmp(fileExt, ".txt")) {
            printf("\nError: invalid outfile name, the file must be a text file!");
            return;
        }

        printf("\nSaving the data inside '%s'...", argv[3]);
        outFile = fopen(argv[3], "w");
    } else {
        char* outFileName = getFileName(argv[1]);
        strcat(outFileName, ".txt");
        printf("\nSaving the data inside '%s'...", outFileName);
        outFile = fopen(outFileName, "w");
        free(outFileName);
    }

    // Write the header line
    fwrite("\n---------    INSTRUCTION    --- I --- OPR ---   ADDRESS    --- ISA ---------\n", 79, 79, outFile);

    for (int i = 0; i < 4096; i++) {
        char* temp = getInstructionInfo(i);
        fwrite(temp, strlen(temp) + 1, strlen(temp) + 1, outFile);
        free(temp);

        // Check for empty instructions
        if (!ram[i] && !ram[(i + 1) % 4096]) {
            // Get the number of empty instructions
            int emptyInstructionsCounter = 0;
            while (!ram[i]) {
                i++;
                emptyInstructionsCounter++;
            }

            temp = (char*) calloc(100, sizeof(char));
            int tempLen = sprintf(temp, "\n *\n *\n *\n Other %d lines of empty instructions\n *\n *\n *\n", emptyInstructionsCounter);
            temp = (char*) realloc(temp, (tempLen + 1));
            fwrite(temp, tempLen + 1, tempLen + 1, outFile);
            free(temp);
        }
    }
    
    // Close the file
    fclose(outFile);

    printf("\nThe instruction have been saved inside the file!");

    return;
}