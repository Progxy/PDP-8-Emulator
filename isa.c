#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "compiler.h"
#include "emulator.h"

bool isISA(char* str) {
    int index = 0;

    // Check if is a label
    if (contains(str, ',')) {
        lcTable[lcIndex] = (str[0] << 8) | str[1];
        lcTable[lcIndex + 1] = (str[2] << 8) | str[3];
        lcTable[lcIndex + 2] = lc;
        lcIndex += 3;

    } else if ((index = startsWith(str, "ORG"))) {
        char hexVal[4];
        int j = 0;

        // Read the hex value
        for (int i = (index + 1); isAHexValue(str[i]) && (j < 4); i++) {
            hexVal[j] = str[i];
            j++;
        }

        lc = strToHex(hexVal, j);
        if (pc == -1) {
            pc = lc;
        }
        return false;
    } else if (startsWith(str, "END")) {
        return true;
    }

    if (pc == -1) {
        pc = lc;
    }
    
    lc++;

    return false;
}

bool isMRI(char* str) {
    int index = 0;
    word instruction = 0;

    if ((index = startsWith(str, "AND"))) {
        // Is not necessary to set the OPR, cause and OPR is 000
    } else if ((index = startsWith(str, "ADD"))) {
        // Set the OPR to 001
        instruction |= (0b001 << 12);
    } else if ((index = startsWith(str, "LDA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "STA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "BUN"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "BSA"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else if ((index = startsWith(str, "ISZ"))) {
        // Set the OPR to 001
        instruction |= (0b010 << 12);
    } else {
        return false;
    }
    
    // Resolve the label once you've known the label position
    instruction = resolveLabel(str, index);
    
    // Save the instruction into the RAM and increment the lc
    ram[lc] = instruction;
    lc++;

    return true;
}

bool isRRI(char* str) {
    if (containsWord(str, "CLA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111100000000000;
    } else if (containsWord(str, "CLE")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111010000000000;
    } else if (containsWord(str, "CMA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111001000000000;
    } else if (containsWord(str, "CME")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000100000000;
    } else if (containsWord(str, "CIR")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000010000000;
    } else if (containsWord(str, "CIL")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000001000000;
    } else if (containsWord(str, "INC")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000100000;
    } else if (containsWord(str, "SPA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000010000;
    } else if (containsWord(str, "SNA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000001000;
    } else if (containsWord(str, "SZA")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000100;
    } else if (containsWord(str, "SZE")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000010;
    } else if (containsWord(str, "HLT")) {
        // Set the instruction into the ram
        ram[lc] = 0b0111000000000001;
    } else {
        return false;
    }

    // Increment the lc
    lc++;

    return true;
}

bool isIO(char* str) {
    if (containsWord(str, "INP")) {
        // Set the instruction into the ram
        ram[lc] = 0b1111100000000000;
    } else if (containsWord(str, "OUT")) {
        // Set the instruction into the ram
        ram[lc] = 0b1111010000000000;
    } else {
        return false;
    }

    // Increment the lc
    lc++;
    return true;
}

bool isPseudoInstruction(char* str) {
    int index = 0;

    if ((index = startsWith(str, "ORG"))) {
        char hexVal[7];
        int j = 0;

        // Read the hex value
        for (int i = (index + 1); isAHexValue(str[i]) && (j < 4); i++) {
            hexVal[j] = str[i];
            j++;
        }

        // Set the lc with the hex value found in the ORG
        lc = strToHex(hexVal, j);

    } else if ((index = startsWith(str, "DEC"))) {
        char val[7];
        int j = 0;

        // Read the dec value
        for (int i = index + 1; str[i] != '\0'; i++) {
            val[j] = str[i];
            j++;
        }

        // Store the value in the ram at the current value of lc and increment the lc
        ram[lc] = parseInt(val, j);
        lc++;

    } else if ((index = startsWith(str, "HEX"))) {
        char hex[7];
        int j = 0;

        for (int i = index + 1; str[i] != '\0'; i++) {
            hex[j] = str[i];
            j++;
        }

        // Store the value in the ram at the current value of lc and increment the lc
        ram[lc] = strToHex(hex, j);
        lc++;

    } else {
        return false;
    }

    return true;
}

void andInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    // MBR <-- M
    mbr = ram[mar];

    // AC <-- AC & MBR
    ac = ac & mbr;

    return;
}

void addInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    // MBR <-- M
    mbr = ram[mar];

    // E-AC <-- AC + MBR
    int temp = ac + mbr;

    // Extract and load the AC and the E registers content
    e = temp >> 16;
    ac = temp & 0b1111111111111111;

    return;
}

void ldaInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    // MBR <-- M
    mbr = ram[mar];

    // AC <-- MBR
    ac = mbr;

    return;
}

void staInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    // MBR <-- AC
    ram[mar] = ac;

    return;
}

void bunInstruction() {
    // PC <-- MBR(ADDR)
    pc = mbr && 0b0111111111111;
    return;
}

void bsaInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    word temp = pc;

    // PC <-- MBR(ADDR)
    pc = mbr && 0b0111111111111;

    // MBR(ADDR) <-- PC
    mbr = pc;

    // M <-- MBR
    ram[mar] = mbr;

    // PC <-- PC + 1
    pc++;

    return;
}

void iszInstruction() {
    // MAR <-- MBR(ADDR)
    word mar = mbr && 0b0111111111111;
    
    // MBR <-- M
    mbr = ram[mar];

    // MBR <-- MBR + 1
    mbr++;
    
    // M <-- MBR
    ram[mar] = mbr;

    // If the MBR is 0 then PC <-- PC + 1
    if (!mbr) {
        pc++;
    }

    return;
}

void claInstruction() {
    // AC <- 0
    ac = 0;
    return;
}

void cleInstruction() {
    // E <- 0
    e = 0;
    return;
}

void cmaInstruction() {
    // AC <- AC'
    ac = ~ac;
    return;
}

void cmeInstruction() {
    // E <- E'
    e = ~e;
    return;
}

void cirInstruction() {
    byte temp = e;

    // E <-- AC(16)
    e = ac & 0b01;
    
    // AC <-- E-AC(1 - 15)
    ac = ac >> 1;
    ac |= (temp << 15);

    return;
}

void cilInstruction() {
    byte temp = e; 

    // E <-- AC(1)
    e = (ac >> 15);

    // AC <-- AC(2 - 16)-E
    ac = ac << 1;
    ac |= e;
    
    return;
}

void incInstruction() {
    // E-AC <-- E-AC + 1
    int temp = ((e << 16) & ac) + 1;

    // Extract and load the AC and the E registers content
    e = temp >> 16;
    ac = temp & 0b1111111111111111;
    return;
}

void spaInstruction() {
    // If AC(1) == 0, then PC <-- PC + 1
    if ((ac >> 15) == 0) {
        pc++;
    }
    return;
}

void snaInstruction() {
    // If AC(1) == 1, then PC <-- PC + 1
    if ((ac >> 15) == 1) {
        pc++;
    }
    return;
}

void szaInstruction() {
    // If AC == 0, then PC <-- PC + 1
    if (ac == 0) {
        pc++;
    }
    return;
}

void szeInstruction() {
    // If E == 0, then PC <-- PC + 1
    if (e == 0) {
        pc++;
    }
    return;
}

void hltInstruction() {
    // S <-- 0
    s = 0;
    return;
}

void inpInstruction() {
    // AC <-- ASCII(Keyboard)
    char c;
    scanf("%c", &c);
    ac = c;

    return;
}

void outInstruction() {
    // Terminal <-- ASCII(AC)
    printf("%c%c", ac >> 8, ac & (0b11111111));
    return;
}
