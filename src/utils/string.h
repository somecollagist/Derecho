#pragma once

#include <screen.h>
#include <types.h>
#include <stdarg.h>

char* itoa(uint64_t num, int base);

void printf(Colour colour, char* str, ...);
