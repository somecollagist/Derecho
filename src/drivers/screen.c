#include "screen.h"

#include <bootinfo.h>
#include <font.h>
#include <memory.h>
#include <types.h>
#include <stdarg.h>

UEFIGraphics_t UEFIGraphics;

uint16_t CharX = 0;
uint16_t CharY = 0;																			// Keeps track of last written character location

typedef struct
{
	uint8_t c1;
	uint8_t c2;
	uint8_t c3;
	uint8_t Reserved;
} __attribute__((packed)) FourByteColourStruct;												// Two modes look like this, but have swapped red and blue channels

void InitScreen()																			// This shouldn't be necessary but is?? DON'T REMOVE!
{
	CharX = 0;
	CharY = 0;
}

void PlotPixel(uint16_t x, uint16_t y, Colour colour)
{
	FourByteColourStruct fbcs;
	switch(UEFIGraphics.PixelFormat)														// Figure out which mode we're in
	{
		case PixelRedGreenBlueReserved8BitPerColor:
			fbcs.c3 = colour.Red;
			fbcs.c2 = colour.Green;
			fbcs.c1 = colour.Blue;
			goto FourBytesPerPixel;
		case PixelBlueGreenRedReserved8BitPerColor:
			fbcs.c3 = colour.Blue;
			fbcs.c2 = colour.Green;
			fbcs.c1 = colour.Red;
			goto FourBytesPerPixel;
		case PixelBitMask:
			goto BitMask;
		default:
			// Throw an error
	}

FourBytesPerPixel:
	fbcs.Reserved = 0;
	*(																						// Load this struct to memory
		(FourByteColourStruct*)
		(UEFIGraphics.FrameBufferBase + 4 * UEFIGraphics.PixelsPerScanLine * y + 4 * x)
	) = fbcs;
	return;

BitMask:																					// Uncertain if this works, docs weren't too clear
	uint32_t ColourBuilder = 0;
	uint8_t RedDepth = 0x80, GreenDepth = 0x80, BlueDepth = 0x80;
	for(int x = 0x80000000; x > 0; x >> 1)
	{
		#define BuildColourBitMask(Mask, Colour, Depth)	\
		if(UEFIGraphics.PixelInformation.Mask && x)		\
		{												\
			ColourBuilder |= (colour.Colour && Depth);	\
			Depth >> 1;									\
			goto EndOfLoop;								\
		}
		BuildColourBitMask(RedMask, Red, RedDepth);
		BuildColourBitMask(GreenMask, Green, GreenDepth);
		BuildColourBitMask(BlueMask, Blue, BlueDepth);
		EndOfLoop:
			continue;
	}
	*(
		(uint32_t*)
		(UEFIGraphics.FrameBufferBase + 4 * UEFIGraphics.PixelsPerScanLine * y + 4 * x)
	) = ColourBuilder;
	return;
}

/// @brief Moves the cursor position to the next location
void CycleToNextCharPosition()
{
	if(++CharX > UEFIGraphics.MaxX)
	{
		CharX = 0;
		if(++CharY > UEFIGraphics.MaxY)
		{
			// Scroll down
		}
	}
}

void PlotCharacter(char c, uint16_t x, uint16_t y, Colour colour)
{
	uint8_t XMask;																			// Bitmap to be used for each row of the character bitmap
	for(int Fy = 0; Fy < FontHeight; Fy++)													// Loop for every row of the character bitmap
	{
		XMask = 1 << (FontWidth-1);															// Set to leftmost bit
		for(int Fx = 0; Fx < FontWidth; Fx++)												// Loop through each column in the bitmap's row
		{
			if((uint8_t)(DerechoBuiltInFont[c].g[Fy]) & XMask)								// If the character bitmap has a bit there, then  plot a pixel
				PlotPixel((x*FontWidth)+Fx, (y*FontHeight)+Fy, colour);
			XMask >>= 1;																	// Shift mask to move along to the right
		}
	}
}

void PlotString(char* s, Colour colour)
{
	while(*s != '\0')																		// Null terminator
	{
		switch(*s)																			// Write any overriding behaviour here
		{
			case '\n':																		// LF is better than CRLF
				CharX = UEFIGraphics.MaxX;													// Set to MaxX, CycleToNextCharPosition will set X to 0 and go to the next line
				break;
			default:
				PlotCharacter(*s, CharX, CharY, colour);									// Plot character by default
				break;
		}
		CycleToNextCharPosition();
		s++;																				// Access next character in the string
	}
}

void printf(Colour colour, char* str, ...)
{
	va_list vl;
	int i = 0, j = 0, k = 0;
	char buffer[256] = {0};
	char temporarybuffer[256] = {0};
	char* temp = &temporarybuffer[0];
	va_start(vl, str);
	while(str && str[i])
	{
		if(str[i] == '%')
		{
			i++;
			k = 0;
			switch(str[i])
			{
				case 'c':	// char
					buffer[j] = (char)va_arg(vl, int);
					j++;
					break;

				case 's':	// string / char*
					char* string = (char*)(va_arg(vl, char*));
					while(*string != '\0')
					{
						buffer[j] = *string;
						j++;
						string++;
					}
					break;
				
				case 'd':	// int (base 10)
					strcpy(itoa(va_arg(vl, int), 10), temp);
					while(*temp != '\0')
					{
						buffer[j] = *temp;
						j++;
						temp++;
					}
					break;

				case 'x':	// int (base 16)
					strcpy(itoa(va_arg(vl, int), 16), temp);
					while(*temp != '\0')
					{
						buffer[j] = *temp;
						j++;
						temp++;
					}
					break;
			}
		}
		else
		{
			buffer[j++] = str[i];
		}
		i++;
	}
	PlotString(buffer, colour);
	va_end(vl);
}
