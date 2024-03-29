#pragma once

#include <types.h>
#include <stdarg.h>

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
#define COLOUR_STDIN	COLOUR_TEAL
#define COLOUR_STDWARN	COLOUR_SALMON
#define COLOUR_STDERR	COLOUR_ORANGE
#define COLOUR_SUCCESS	COLOUR_GREEN
#define COLOUR_FATAL	COLOUR_RED
#define COLOUR_PROCESS	COLOUR_AZURE

/// @brief Initialises screen
void InitScreen();

/// @brief Plots a pixel on the screen
/// @param x x-coordinate to plot pixel to
/// @param y y-coordinate to plot pixel to
/// @param colour Colour of pixel to be plotted
void PlotPixel(uint16_t x, uint16_t y, Colour colour);
/// @brief Writes a character to the screen
/// @param c Character to write
/// @param x x-coordinate to write character to
/// @param y y-coordinate to write character to
/// @param colour Colour of character to write
void PlotCharacter(char c, uint16_t x, uint16_t y, Colour colour);
/// @brief Writes a string to the screen
/// @param s String to write
/// @param colour Colour of string to write
void PlotString(char* s, Colour colour);
/// @brief Formatted print
/// @param colour Colour to print text as
/// @param str String to print out, may be infixed with %
/// @param ... Extra values to infix to the string, denoted by %
void printf(Colour colour, char* str, ...);