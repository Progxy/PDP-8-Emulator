typedef int bool;
typedef short word;
#define false 0;
#define true 1;
#define min(a,b) (a <= b ? a : b)

/// @brief Check if the given string contains the given char.
/// @param str 
/// @param c 
/// @return Return true if it does contain the char, otherwise return false.
bool contains(char* str, char c);

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

/// @brief Convert a char to the corresponding hex value.
/// @param c 
/// @return Return the hex value.
word charToHex(char c);

/// @brief Convert the given string to the corresponding hex value.
/// @param str 
/// @return Return the hex value.
word strToHex(char* str);
