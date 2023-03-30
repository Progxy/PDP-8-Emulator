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

int startsWith(char* str, char* cmp) {
    int index = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == cmp[index]) {
            index++;
            if ((index + 1) == strlen(cmp)) {
                return i;
            }
            continue;
        }
        index = 0;
    }
    return false;
}

bool compareStrings(char* str1, char* str2) {
    int len = min(strlen(str1), strlen(str2));
    for (int i = 0; i < len; i++) {
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

static word charToHex(char c) {
    if ((c >= 65) && (c <= 70)) {
        return (word) (c - 55);
    } else if ((c >= 97) && (c <= 102)) {
        return (word) (c - 87);
    } else if ((c >= 48) && (c <= 57)) {
        return (word) (c - 48);
    } else {
        return 0x00;
    }
}

static word charToInt(char c) {
    if ((c >= 48) && (c <= 57)) {
        return (word) (c - 48);
    } else {
        return 0;
    }
}

word strToHex(char* str, int len) {
    word hex = 0x00;
    int posExp = pow(16, len - 1);

    for (int i = 0; i < len; i++) {
        hex += posExp * charToHex(str[i]);
        posExp /= 16;
    }

    return hex;
}

word parseInt(char* str, int len) {
    word integer = 0x00;
    int posExp = pow(10, len - 1);

    for (int i = 0; i < len; i++) {
        integer += posExp * charToHex(str[i]);
        posExp /= 10;
    }

    return integer;
}
