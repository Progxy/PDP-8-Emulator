#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

bool contains(char* str, char c) {
    for (int i = 0; i < 4; i++) {
        if (str[i] == "") {
            return true;
        }
    }
    return false;
}

bool compareStrings(char* str1, char* str2) {
    int len = min(strlen(str1), strlen(str2));
    for (int i = 0; i < 4; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

int pow(int base, int exp) {
    if (exp == 0) {
        return 1;
    }

    for (int i = exp; i > 1; i--) {
        base *= base;
    }

    return base;
}

word charToHex(char c) {
    if ((c >= 65) && (c <= 70)) {
        return (word) (c - 55);
    } else if ((c >= 97) && (c <= 102)) {
        return (word) (c - 55);
    } else if ((c >= 48) && (c <= 57)) {
        return (word) (c - 48);
    } else {
        return 0x00;
    }
}

word strToHex(char* str) {
    word hex = 0x00;
    int posExp = pow(16, strlen(str) - 1);

    for (int i = 0; i < strlen(str); i++) {
        hex += posExp * charToHex(str[i]);
        posExp /= 16;
    }

    return hex;
}
