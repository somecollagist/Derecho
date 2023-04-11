#pragma once

#include <types.h>
#include <efi.h>
#include <efilib.h>

typedef struct 
{
	uint64_t				MapSize;
	EFI_MEMORY_DESCRIPTOR*	Map;
	uint64_t				MapKey;
	uint64_t				DescriptorSize;
	uint32_t				DescriptorVersion;
} UEFIMemoryMap;

extern UEFIMemoryMap UEFIMMap;

void InitialiseMemoryMap();