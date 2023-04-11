#pragma once

#include <types.h>
#include <efi.h>
#include <efilib.h>

typedef struct
{
	uint64_t FrameBufferBase;
	uint64_t FrameBufferSize;
	uint64_t HorizontalResolution;
	uint64_t VerticalResolution;
	uint64_t PixelsPerScanLine;
	EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
	EFI_PIXEL_BITMASK PixelInformation;
	uint16_t MaxX;
	uint16_t MaxY;
} UEFIGraphics;

extern UEFIGraphics UEFIGraph;

void InitialiseGraphics();