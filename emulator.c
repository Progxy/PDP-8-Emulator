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
extern bool step;
word ac = 0;
word mbr = 0;
byte opr = 0;
byte e = 0;
byte i = 0;
byte s = 0;

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
    word mar = mbr && 0b0111111111111;
    
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
            if (step) {
                printf("\nExecuting AND instruction!");
            }
            andInstruction();
            break;
        
        case 0b001:
            if (step) {
                printf("\nExecuting ADD instruction!");
            }
            addInstruction();
            break;
        
        case 0b010:
            if (step) {
                printf("\nExecuting LDA instruction!");
            }
            ldaInstruction();
            break;
        
        case 0b011:
            if (step) {
                printf("\nExecuting STA instruction!");
            }
            staInstruction();
            break;

        case 0b100:
            if (step) {
                printf("\nExecuting BUN instruction!");
            }
            bunInstruction();
            break;

        case 0b101:
            if (step) {
                printf("\nExecuting BSA instruction!");
            }
            bsaInstruction();
            break;
        
        case 0b110:
            if (step) {
                printf("\nExecuting ISZ instruction!");
            }
            iszInstruction();
            break;

        case 0b111:
            // Switch between IO instructions
            if (i) {
                if (mbr & 0b0000100000000000) {
                    if (step) {
                        printf("\nExecuting INP instruction!");
                    }
                    inpInstruction();
                } else if (mbr & 0b0000010000000000) {
                    if (step) {
                        printf("\nExecuting OUT instruction!");
                    }
                    outInstruction();
                }
                break;
            }

            // Switch between RRI instructions
            switch (mbr & 0b0000111111111111) {
                case 0b100000000000:
                    if (step) {
                        printf("\nExecuting CLA instruction!");
                    }
                    claInstruction();
                    break;

                case 0b010000000000:
                    if (step) {
                        printf("\nExecuting CLE instruction!");
                    }
                    cleInstruction();
                    break;

                case 0b001000000000:
                    if (step) {
                        printf("\nExecuting CMA instruction!");
                    }
                    cmaInstruction();
                    break;

                case 0b000100000000:
                    if (step) {
                        printf("\nExecuting CME instruction!");
                    }
                    cmeInstruction();
                    break;

                case 0b000010000000:
                    if (step) {
                        printf("\nExecuting CIR instruction!");
                    }
                    cirInstruction();
                    break;

                case 0b000001000000:
                    if (step) {
                        printf("\nExecuting CIL instruction!");
                    }
                    cilInstruction();
                    break;

                case 0b000000100000:
                    if (step) {
                        printf("\nExecuting INC instruction!");
                    }
                    incInstruction();
                    break;

                case 0b000000010000:
                    if (step) {
                        printf("\nExecuting SPA instruction!");
                    }
                    spaInstruction();
                    break;

                case 0b000000001000:
                    if (step) {
                        printf("\nExecuting SNA instruction!");
                    }
                    snaInstruction();
                    break;

                case 0b000000000100:
                    if (step) {
                        printf("\nExecuting SZA instruction!");
                    }
                    szaInstruction();
                    break;

                case 0b000000000010:
                    if (step) {
                        printf("\nExecuting SZE instruction!");
                    }
                    szeInstruction();
                    break;

                case 0b000000000001:
                    if (step) {
                        printf("\nExecuting HLT instruction!");
                    }
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
    printf("\nPC register: ");
    printBits(pc, getBitSize(pc));
    printf("\nAC register: ");
    printBits(ac, getBitSize(ac));
    printf("\nMBR register: ");
    printBits(mbr, getBitSize(mbr));
    printf("\nOPR register: ");
    printBits(opr, getBitSize(opr));
    printf("\nE register: ");
    printBits(e, getBitSize(e));
    printf("\nI register: ");
    printBits(i, getBitSize(i));
    printf("\nS register: ");
    printBits(s, getBitSize(s));
    return;
}

void emulate() {
    // Check if the machine is on
    if (s == 0) {
        printf("\nShutting down the machine.");
        return;
    } 
    
    // If the step mode is active, then wait the user's confirm to continue with the next cycle
    if (step) {
        printMachineInfo();
        askContinue("Do you want to step to the next instruction ? (Y/N): ");
        printf("\n");
    }

    // Switch between the four cycles
    switch (cycle) {
        case 0:
            if (step) {
                printf("\nEntering the fetch cycle!");
            }
            fetchCycle();
            cycle++;
            break;
        
        case 1:
            if (step) {
                printf("\nEntering the IMA cycle!");
            }
            IMACycle();
            cycle++;
            break;        
            
        case 2:
            if (step) {
                printf("\nEntering the execute cycle!");
            }
            executeCycle();
            cycle++;
            break;        
    }

    return emulate(step);
}

void toggleMachineState() {
    s = !s;
    printf("\nThe machine is now: %s", s ? "ON" : "OFF");
    return;
}
