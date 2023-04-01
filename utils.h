typedef int bool;
typedef short word;
typedef char byte;
#define false 0;
#define true 1;
#define min(a,b) (a <= b ? a : b)
#define isAHexValue(c) (((c >= 65) && (c <= 70)) || ((c >= 97) && (c <= 102)) ||((c >= 48) && (c <= 57)))

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
int pow(int base, int exp);

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
