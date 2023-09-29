#pragma once

#include <types.h>

/// @brief Converts an integer into a number of the given base
/// @param num Number to convert
/// @param base Base to convert to, must be between 2 and 16 inclusive
/// @param length Minimum length to make the resulting string, padded with leading zeroes
/// @return Returns a string representing the number in the given base
char* itoa(uint64_t num, uint8_t base, uint8_t length);

/// @brief Counts the length of a null terminated string
/// @param str The null terminated string that you want to get the length of
/// @return Returns the length of the string minus the null character
size_t strlen(const char* str);
