# Pdp8 Emulator

This is a simple PDP8 Emulator written in C, that compiles and runs files written in PDP8 Assembly Language (.asm/.s/.S/.pdp8).

## Features
This features are currently supported:

* compile the given assembly file and analyze the instructions;

* dump into a text file the instructions to analyze them after;

* emulate the assembly file after compiling it;

* step through each step of the execution.

## Assembler Features

The assembler currently supports this instructions: 

* ### Memory Reference Instructions (MRI)
    | KEYWORD | I REGISTER | OPR REGISTER | ADDRESS | FUNCTION |
    |---------|------------|--------------|---------|----------|  
    |AND|0/1|000|12 bits address|And operation between the value at the specified address and the AC
    |ADD|0/1|001|12 bits address|Add operation between the value at the specified address and the AC 
    |LDA|0/1|010|12 bits address|Load into the AC the value at the specified address
    |STA|0/1|011|12 bits address|Store at the specified address the value stored inside the AC
    |BUN|0/1|100|12 bits address|Jump unconditionally to the specified address
    |BSA|0/1|101|12 bits address|Jump to the specified address plus one and store the current PC value at the specified address
    |ISZ|0/1|110|12 bits address|Increment the value inside the specified address and skip the next instruction if the incremented value is equal to zero

* ### Register Reference Instructions (RRI)
    | KEYWORD | I REGISTER | OPR REGISTER | ADDRESS | FUNCTION |
    |---------|------------|--------------|---------|----------|  
    |CLA|0|111|100000000000|Clear the AC by filling it with zeros|
    |CLE|0|111|010000000000|Clear the E Register by setting it to zero|
    |CMA|0|111|001000000000|Complement the AC|
    |CME|0|111|000100000000|Complement the E Register|
    |CIR|0|111|000010000000|Circulate/Shift to the right the bits inside the AC, including the bit inside the E Register as the 17th bit of the AC|
    |CIL|0|111|000001000000|Circulate/Shift to the left the bits inside the AC, including the bit inside the E Register as the 17th bit of the AC|
    |INC|0|111|000000100000|Increment the AC|
    |SPA|0|111|000000010000|Skip the next instruction if the value inside the AC is POSITIVE|
    |SNA|0|111|000000001000|Skip the next instruction if the value inside the AC is NEGATIVE|
    |SZA|0|111|000000000100|Skip the next instruction if the value inside the AC is ZERO|
    |SZE|0|111|000000000010|Skip the next instruction if the value inside the E Register is ZERO|
    |HLT|0|111|000000000001|Stop the execution and shutdown the machine, by setting the S Register to 0|

* ### Input/Output Instructions (IOI)
    | KEYWORD | I REGISTER | OPR REGISTER | ADDRESS | FUNCTION |
    |---------|------------|--------------|---------|----------| 
    |INP|1|111|100000000000|
    |OUT|1|111|010000000000|

### Notes
* The I Register data in the MRI instructions is used to determine, whether or not, the Indirect Memory Addressing should be used;
* OPR REGISTER = "OPERATION CODE 3-bit Register";
* I REGISTER = "INDIRECT MEMORY ADDRESSING bit Register";
* E Register = "The Extension Register is used as the 17th bit of the Accumulator, therefore is called Extension Register, but can be used as a single bit register";
* S Register = "The Status Register is a single bit register used to indicate the current status of the machine (1 = ON/0 = OFF)";
* PC = "Program Counter, 12-bit Register";
* AC = "Accumulator, 16-bit Register".

---

Both the assembler and the emulator are based on the PDP-8 version used for teaching at the University of Perugia.