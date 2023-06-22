#pragma once

#include <types.h>

/// @brief Converts an integer into a number of the given base
/// @param num Number to convert
/// @param base Base to convert to, must be between 2 and 16 inclusive
/// @return Returns a string representing the number in the given base
char* itoa(uint64_t num, uint8_t base);