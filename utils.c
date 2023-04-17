#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void printHex(word value, int size) {
    const char hexDictionary[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    // Print the hex identifier
    printf(" 0x");
    
    for (int i = size - 1; i >= 0; i--) {
        // Print the value of the last 4 bits
        printf("%c", hexDictionary[((value >> (4 * i)) & 0b1111)]);
    }

    return;
}

char* trimString(char* str) {
    // Find the number of whitespaces
    int index = 0;
    while (str[index] == ' ') {
        index++;
    }

    int lastIndex = strlen(str) - 1; 
    while(str[lastIndex] == ' ') {
        lastIndex--;
    }

    // Shift all the non-whitespaces to the start of the string
    int j = 0;
    char* newString = (char*) calloc(1, sizeof(char));
    for (int i = index; i <= lastIndex; i++) {
        newString[j] = str[i];
        j++;
        newString = (char*) realloc(newString, sizeof(char) * (j + 1));
    }

    // Add the string terminator character
    newString[j] = '\0';

    // Free the given string
    free(str);

    return newString;
}

void printBits(long long val, int limit) {
    if (limit == -1) {
        return;
    }
    printf("%d%s", (char) (val >> limit) & 0b01, (limit % 8) ? "" : " ");
    return printBits(val, limit - 1);
}

void cleanStdin() {
    unsigned char c;

    while ((c = getc(stdin)) != EOF) {
        if (c == '\n') {
            break;          
        }
    } 
    
    return;
}

void askContinue(char* str) {
    // Print the given text
    printf("\n%s", str);
    
    // Prompt the user
    char confirm;
    scanf("%c", &confirm);

    // Clean the stdin if the user inputs something else than '\n'
    if (confirm != '\n') {
        return cleanStdin();
    }
    return;
}

bool compareLabels(int label, char* str) {
    for (int i = 0; i < 4; i++) {
        if (str[i] != ((label >> (24 - (i * 8))) && (0b11111111))) {
            return false;
        }
    }
    return true;
}

void formatLabel(char* str, int len) {
    str[len] = ',';
    for (int i = len + 1; i < 4; i++) {
        str[i] = ' ';
    }
    return;
}


bool contains(char* str, char c) {
    for (int i = 0; (str[i] != '\0'); i++) {
        if (str[i] == c) {
            return true;
        }
    }
    return false;
}

bool containsWord(char* str, char* cmp) {
    int index = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == cmp[index]) {
            index++;
            if (index == strlen(cmp)) {
                return true;
            }
            continue;
        }
        index = 0;
    }
    
    return false;
}

int startsWith(char* str, char* cmp) {
    int index = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == cmp[index]) {
            index++;
            if (index == strlen(cmp)) {
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

int power(int base, int exp) {
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
    int posExp = power(16, len - 1);
    bool isNegative = false;

    for (int i = 0; i < len; i++) {
        if (str[i] == '-') {
            isNegative = true;
            posExp /= 16;
            continue;
        }
        hex += posExp * charToHex(str[i]);
        posExp /= 16;
    }

    hex *= isNegative ? -1 : 1;

    return hex;
}

word parseInt(char* str, int len) {
    word integer = 0x00;
    int posExp = power(10, len - 1);
    bool isNegative = false;

    for (int i = 0; i < len; i++) {
        if (str[i] == '-') {
            isNegative = true;
            posExp /= 10;
            continue;
        }
        integer += posExp * charToInt(str[i]);
        posExp /= 10;
    }

    integer *= isNegative ? -1 : 1;

    return integer;
}

int logaritm(int val) {
    int result = 0;
    for (result = 0; val != 1; result++) {
        val = val >> 1;
    }
    return result;
}
