#pragma once

#include <types.h>

typedef struct
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} Colour;

#define COLOUR(r, g, b) (Colour){r,g,b}

// Flashbacks to art, I'm shaken. See colours.svg

#define COLOUR_BLACK	COLOUR(0x00, 0x00, 0x00)
#define COLOUR_NAVY		COLOUR(0x00, 0x00, 0x88)
#define COLOUR_BLUE		COLOUR(0x00, 0x00, 0xFF)
#define COLOUR_LAUREL	COLOUR(0x00, 0x88, 0x00)
#define COLOUR_TEAL		COLOUR(0x00, 0x88, 0x88)
#define COLOUR_AZURE	COLOUR(0x00, 0x88, 0xFF)
#define COLOUR_GREEN	COLOUR(0x00, 0xFF, 0x00)
#define COLOUR_LIME		COLOUR(0x00, 0xFF, 0x88)
#define COLOUR_CYAN		COLOUR(0x00, 0xFF, 0xFF)
#define COLOUR_CRIMSON	COLOUR(0x88, 0x00, 0x00)
#define COLOUR_PURPLE	COLOUR(0x88, 0x00, 0x88)
#define COLOUR_VIOLET	COLOUR(0x88, 0x00, 0xFF)
#define COLOUR_OLIVE	COLOUR(0x88, 0x88, 0x00)
#define COLOUR_GREY		COLOUR(0x88, 0x88, 0x88)
#define COLOUR_MALIBU	COLOUR(0x88, 0x88, 0xFF)
#define COLOUR_MUSTARD	COLOUR(0x88, 0xFF, 0x00)
#define COLOUR_MINT		COLOUR(0x88, 0xFF, 0x88)
#define COLOUR_AQUA		COLOUR(0x88, 0xFF, 0xFF)
#define COLOUR_RED		COLOUR(0xFF, 0x00, 0x00)
#define COLOUR_PINK		COLOUR(0xFF, 0x00, 0x88)
#define COLOUR_MAGENTA	COLOUR(0xFF, 0x00, 0xFF)
#define COLOUR_ORANGE	COLOUR(0xFF, 0x88, 0x00)
#define COLOUR_SALMON	COLOUR(0xFF, 0x88, 0x88)
#define COLOUR_ROSE		COLOUR(0xFF, 0x88, 0xFF)
#define COLOUR_YELLOW	COLOUR(0xFF, 0xFF, 0x00)
#define COLOUR_CANARY	COLOUR(0xFF, 0xFF, 0x88)
#define COLOUR_WHITE	COLOUR(0xFF, 0xFF, 0xFF)

#define COLOUR_STDOUT	COLOUR_WHITE
#define COLOUR_STDIN	COLOUR_CYAN
#define COLOUR_STDERR	COLOUR_RED
#define COLOUR_SUCCESS	COLOUR_GREEN
#define COLOUR_FATAL	COLOUR_CRIMSON
#define COLOUR_PROCESS	COLOUR_AZURE

void InitScreen();
void PlotPixel(uint16_t x, uint16_t y, Colour colour);
void PlotCharacter(char c, uint16_t x, uint16_t y, Colour colour);
void PlotString(char* s, Colour colour);