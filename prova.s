// Decremento Iterativo
ORG 100
LDA IN
STA TMP
LOP, ISZ CNT
BSA DCR
LDA TMP
OUT
HLT

// Decr subroutine
ORG 200 ;comment
DCR, DEC 0 # DEC 0
LDA TMP
ADD M1
STA TMP
BUN LOP

// Init variables
ORG 300
IN, DEC 75
M1, DEC -1
CNT, DEC -11
TMP, DEC 0

END
