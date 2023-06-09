#pragma once

#include <efi.h>
#include <efilib.h>
#include <types.h>

typedef struct 
{
	uint64_t					FrameBufferBase;
	uint64_t					FrameBufferSize;
	uint64_t					HorizontalResolution;
	uint64_t					VerticalResolution;
	uint64_t					PixelsPerScanLine;
	EFI_GRAPHICS_PIXEL_FORMAT	PixelFormat;
	EFI_PIXEL_BITMASK			PixelInformation;
	uint16_t					MaxX;
	uint16_t					MaxY;
} UEFIGraphics_t;

typedef struct 
{
	uint64_t					MapSize;
	EFI_MEMORY_DESCRIPTOR*		Map;
	uint64_t					MapKey;
	uint64_t					DescriptorSize;
	uint32_t					DescriptorVersion;
} UEFIMemoryMap_t;

typedef struct
{
	UEFIGraphics_t*				UEFIGraphics;
	UEFIMemoryMap_t*			UEFIMemoryMap;
	uint64_t					EFIConfigEntries;
	uint64_t					EFIConfigAddr;
} BootInfo;

/// @brief Unpacks the bootinfo from the bootloader to necessary structs
/// @param bootinfo Pointer to the bootinfo struct passed to kernel main from the EFI bootloader
void UnpackBootInfo(BootInfo* bootinfo);

extern UEFIGraphics_t			UEFIGraphics;
extern UEFIMemoryMap_t			UEFIMemoryMap;
extern uint64_t					EFIConfigEntries;
extern uint64_t					EFIConfigAddr;