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

                default:
                    break;
            }

            break;
        
        default:
            break;
    }

    // Go to the fetch cycle
    cycle = 0;

    return;
}

void emulate(bool step) {
    // Check if the machine is on
    if (s == 0) {
        printf("\nShutting down the machine.");
        return;
    } 
    
    // If the step mode is active, then wait the user's confirm to continue with the next cycle
    if (step) {
        askContinue("Do you want to step to the next instruction ? (Y/N): ");
        printf("\n");
    }

    // Switch between the four cycles
    switch (cycle) {
        case 0:
            fetchCycle();
            cycle++;
            break;
        
        case 1:
            IMACycle();
            cycle++;
            break;        
            
        case 2:
            executeCycle();
            cycle++;
            break;        
        
        default:
            break;
    }

    return emulate(step);
}

void toggleMachineState() {
    s = !s;
    printf("\nThe machine is now: %s", s ? "ON" : "OFF");
    return;
}
