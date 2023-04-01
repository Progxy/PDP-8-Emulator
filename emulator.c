#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "isa.h"

static int tick = 0;
static int cycle = 0;

void emulate() {
    // Map the cycle counter between 0 and 3
    cycle = cycle % 4;

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
            executionCycle();
            cycle++;
            break;        
            
        case 3:
            interruptCycle();
            cycle++;
            break;
        
        default:
            break;
    }

    return runProgram();
}
