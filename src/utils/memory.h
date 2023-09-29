#pragma once

#include <types.h>

/// @brief Copies data from one memory location to another
/// @param src Source memory location (data to be copied)
/// @param dst Destination memory location (place for data to be copied to)
void strcpy(char* src, char* dst);

/// @brief Copies memory from one location to another, this does not handle overlapping memory regions
/// @param src Source memory location (data to be copied)
/// @param dst Destination memory location (place for data to be copied to)
/// @param count The amount of bytes to be copied
void memcpy(const void* restrict src, void* restrict dst, size_t count);

/// @brief Copies memory from one location to another, this handles overlapping memory regions
/// @param src Source memory location (data to be copied)
/// @param dst Destination memory location (place for data to be copied to)
/// @param count The amount of bytes to be copied
void memmove(const void* src, void* dst, size_t count);
