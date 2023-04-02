#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "emulator.h"
#include "utils.h"
#include "isa.h"

static int cycle = 0;

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
    return;
}
