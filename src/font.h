#pragma once

#include <types.h>

#define FontWidth 8
#define FontHeight 16

typedef struct { uint8_t g[FontHeight]; } Glyph;

#define GLYPH(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) (Glyph){{_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16}}
#define EMPTY_GLYPH (Glyph){{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}

/// @brief The built in font for Derecho, easy to modify
extern Glyph DerechoBuiltInFont[];