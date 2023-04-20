#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"
#include "isa.h"

static int cycle = 0;
extern word ram[4096];
extern word pc;
bool stepFlag;
word ac = 0;
word mbr = 0;
byte opr = 0;
byte e = 0;
byte i = 0;
byte s = 0;
extern char* mriInstructions[];
extern char* rriInstructions[];    
extern char* ioInstructions[];
static const char* cyclesNames[] = {"FETCH", "IMA", "EXECUTE", "-"};
static int currentCycle = 3;
static int currentInstruction = 0;

static void fetchCycle() {
    // Read the instruction from the memory
    mbr = ram[pc];
    
    // PC <-- PC + 1
    pc++;
    
    // OPR <-- MBR(OPR)
    opr = (mbr & (0b0111000000000000)) >> 12;
    
    // I <-- MBR(I)
    i = (mbr & (0b1000000000000000)) >> 15;

    // If the instruction is an MRI and the I register is on then jump to the IMA cycle
    if (i && (opr != 0b0111)) {
        cycle++;
        return;
    }

    // Else jump to the execute cycle
    cycle = 2;

    return;
}

static void IMACycle() {
    // MAR <-- MBR(ADDR)
    word mar = mbr & 0b0111111111111;

    // MBR <-- M
    mbr = ram[mar];

    // Go to the execute cycle
    cycle++;

    return;
}

static void executeCycle() {
    // Execute the instruction based on the content of the OPR
    switch (opr) {
        case 0b000:
            andInstruction();
            break;
        
        case 0b001:
            addInstruction();
            break;
        
        case 0b010:
            ldaInstruction();
            break;
        
        case 0b011:
            staInstruction();
            break;

        case 0b100:
            bunInstruction();
            break;

        case 0b101:
            bsaInstruction();
            break;
        
        case 0b110:
            iszInstruction();
            break;

        case 0b111:
            // Switch between IO instructions
            if (i) {
                if (mbr & 0b0000100000000000) {
                    inpInstruction();
                } else if (mbr & 0b0000010000000000) {
                    outInstruction();
                }
                break;
            }

            // Switch between RRI instructions
            switch (mbr & 0b0000111111111111) {
                case 0b100000000000:
                    claInstruction();
                    break;

                case 0b010000000000:
                    cleInstruction();
                    break;

                case 0b001000000000:
                    cmaInstruction();
                    break;

                case 0b000100000000:
                    cmeInstruction();
                    break;

                case 0b000010000000:
                    cirInstruction();
                    break;

                case 0b000001000000:
                    cilInstruction();
                    break;

                case 0b000000100000:
                    incInstruction();
                    break;

                case 0b000000010000:
                    spaInstruction();
                    break;

                case 0b000000001000:
                    snaInstruction();
                    break;

                case 0b000000000100:
                    szaInstruction();
                    break;

                case 0b000000000010:
                    szeInstruction();
                    break;

                case 0b000000000001:
                    hltInstruction();
                    break;

            }

            break;
    }

    // Go to the fetch cycle
    cycle = 0;

    return;
}

/// @brief Print the content of every register.
static void printMachineInfo() {
    printf("\n------------------- REGISTERS INFO -------------------\n");
    printf("\nPC register:  ");
    printBits(pc, getBitSize(pc));
    printHex(pc, getHexSize(pc));
    printf("\nAC register:  ");
    printBits(ac, getBitSize(ac));
    printHex(ac, getHexSize(ac));
    printf("\nMBR register: ");
    printBits(mbr, getBitSize(mbr));
    printHex(mbr, getHexSize(mbr));
    printf("\nOPR register:          ");
    printBits(opr, getBitSize(opr));
    printHex(opr, getHexSize(opr));
    printf("\nE register:            ");
    printBits(e, getBitSize(e));
    printHex(e, getHexSize(e));
    printf("\nI register:            ");
    printBits(i, getBitSize(i));
    printHex(i, getHexSize(i));
    printf("\nS register:            ");
    printBits(s, getBitSize(s));
    printHex(s, getHexSize(s));
    printf("\nCurrent cycle: %s", cyclesNames[currentCycle]);
    if (currentCycle == 2) {
        printf(" - (%s%s%s Instruction)", mriInstructions[(int) opr], (opr == 7 && !i) ? rriInstructions[logaritm(currentInstruction)] : "", (opr == 7 && i) ? ioInstructions[logaritm(currentInstruction) - 10] : "");
    }
    printf("\nNext cycle: %s", cyclesNames[cycle]);
    if (cycle == 2) {
        printf(" - (%s%s%s Instruction)", mriInstructions[(int) opr], (opr == 7 && !i) ? rriInstructions[logaritm(mbr & 0b0000111111111111)] : "", (opr == 7 && i) ? ioInstructions[logaritm(mbr & 0b0000111111111111) - 10] : "");
    }
    printf("\n------------------------------------------------------\n");

    // Update currentCycle and currentInstruction
    currentCycle = cycle;
    currentInstruction = mbr & 0b0000111111111111;

    return;
}

void initEmulator(bool flags) {
    // Set the given flag
    stepFlag = flags;
    
    // Turn on the machine
    toggleMachineState();
    
    // Print the start of the output of the emulator
    printf("\n------- OUTPUT: -------\n");

    return;
}

void emulate() {
    // Check if the machine is on
    if (s == 0) {
        printf("\nShutting down the machine.");
        printf("\n-----------------------\n");
        return;
    } 
    
    // If the stepFlag mode is active, then wait the user's confirm to continue with the next cycle
    if (stepFlag) {
        printMachineInfo();
        askContinue("Press ENTER to step to the next instruction: ");
    }

    // Switch between the four cycles
    switch (cycle) {
        case 0:
            fetchCycle();
            break;
        
        case 1:
            IMACycle();
            break;        
            
        case 2:
            executeCycle();
            break;        
    }

    return emulate();
}

void toggleMachineState() {
    s = !s;
    printf("\nThe machine is now: %s\n", s ? "ON" : "OFF");
    return;
}
