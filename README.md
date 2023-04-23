# Pdp8 Emulator

This is a simple PDP8 Emulator written in C, that compiles and run files written using the PDP8 Assembly Language (.asm/.s/.S/.pdp8).

## Features
This features are currently supported:
* compile the given assembly file and analyze the instructions;

* dump into a text file the instructions to analyze them after;

* emulate the assembly file after compiling it;

* step through each step of the execution.

## Assembler Features

The assembler currently supports this instructions: 

* ### Memory Reference Instructions (MRI)
    | NAME | I REGISTER | OPR REGISTER | ADDRESS | FUNCTION |
    |------|------------|--------------|---------|----------|  
    |AND|0/1|000|12 bits address|And operation between the value at the specified address and the AC
    |ADD|0/1|001|12 bits address|Add operation between the value at the specified address and the AC 
    |LDA|0/1|010|12 bits address|Load into the AC the value at the specified address
    |STA|0/1|011|12 bits address|Store at the specified address the value stored inside the AC
    |BUN|0/1|100|12 bits address|Jump unconditionally to the specified address
    |BSA|0/1|101|12 bits address|Jump to the specified address plus one and store the current PC value at the specified address
    |ISZ|0/1|110|12 bits address|Increment the value inside the specified address and skip the next instruction if the incremented value is equal to zero

* ### Register Reference Instructions (RRI)
    | NAME | I REGISTER | OPR REGISTER | ADDRESS |
    |------|------------|--------------|---------|  
    |CLA|0|111|100000000000|
    |CLE|0|111|010000000000|
    |CMA|0|111|001000000000|
    |CME|0|111|000100000000|
    |CIR|0|111|000010000000|
    |CIL|0|111|000001000000|
    |INC|0|111|000000100000|
    |SPA|0|111|000000010000|
    |SNA|0|111|000000001000|
    |SZA|0|111|000000000100|
    |SZE|0|111|000000000010|
    |HLT|0|111|000000000001|

* ### Input/Output Instructions (IOI)
    | NAME | I REGISTER | OPR REGISTER | ADDRESS |
    |------|------------|--------------|---------|  
    |INP|1|111|100000000000|
    |OUT|1|111|010000000000|

### Notes
* The I Register data in the MRI instructions is used to determine, whether or not, the Indirect Memory Addressing should be used;
* PC = "Program Counter, 12-bit Register";
* AC = "Accumulator, 16-bit Register".