#pragma once

#include <string.h>

typedef int bool;
typedef short word;
typedef char byte;
#define false 0;
#define true 1;
#define min(a,b) (a <= b ? a : b)
#define isAHexValue(c) (((c >= 65) && (c <= 70)) || ((c >= 97) && (c <= 102)) ||((c >= 48) && (c <= 57)))
#define getBitSize(val) ((sizeof(val) * 8) - 1)
#define endsWith(str, c) (str[strlen(str) - 1] == c)

/// @brief Trim the given string.
/// @param str 
/// @return Return the len of the string if starts with a whitespace, otherwise return 0.
void trimString(char* str);

/// @brief Calculate the log of the given value in base 2.
/// @param val 
/// @return Return the log of the given value in base 2.
int logaritm(int val);

/// @brief Print the given value as a binary string.
/// @param val 
/// @param limit 
void printBits(long long val, int limit);

/// @brief Ask if the user wants to continue.
void askContinue(char* str);

/// @brief Check if the given string matches the given label.
/// @param label 
/// @param str 
/// @return Return true if the given string does match the given label.
bool compareLabels(int label, char* str);

/// @brief Format the given string to match the structure of the labels that are saved in the table.
/// @param str 
/// @param len 
void formatLabel(char* str, int len);

/// @brief Check if the given string contains the given char.
/// @param str 
/// @param c 
/// @return Return true if it does contain the char, otherwise return false.
bool contains(char* str, char c);

/// @brief Check if the given string contains the given string.
/// @param str 
/// @param cmp 
/// @return Return true if the substring matches the string, otherwise return false.
bool containsWord(char* str, char* cmp);

/// @brief Check if the given string starts with the given string.
/// @param str 
/// @param cmp 
/// @return Return the position of the end of the given string, otherwise return false.
bool startsWith(char* str, char* cmp);

/// @brief Compare the given strings.
/// @param str1 
/// @param str2 
/// @return Return true if the strings match, otherwise return false.
bool compareStrings(char* str1, char* str2);

/// @brief Calculate the power of a number.
/// @param base 
/// @param exp 
/// @return Return the power of the given number.
int power(int base, int exp);

/// @brief Convert the given string to the corresponding hex value.
/// @param str 
/// @param len
/// @return Return the hex value.
word strToHex(char* str, int len);

/// @brief Convert the given string to the corresponding int value.
/// @param str 
/// @param len 
/// @return Return the int value.
word parseInt(char* str, int len);
