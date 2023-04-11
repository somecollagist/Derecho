#include "memory.h"

#include <types.h>
#include <efi.h>
#include <efilib.h>

UEFIMemoryMap UEFIMMap;
UINT8 MemoryInitialised = 0;

void InitialiseMemoryMap()
{
	if(MemoryInitialised) return;

	MemoryInitialised = 1;													// Memory never gets initialised more than once

	UEFIMMap.MapSize = sizeof(EFI_MEMORY_DESCRIPTOR) * 64;					// Try this size for storing the memory map
	EFI_STATUS Status;
	while(1)																// Try storing the memory map in this size
	{
		Status = uefi_call_wrapper(
			ST->BootServices->AllocatePool, 3,
			EfiLoaderData,
			UEFIMMap.MapSize
		);
		Status = uefi_call_wrapper(
			ST->BootServices->GetMemoryMap, 5,
			&UEFIMMap.MapSize,
			UEFIMMap.Map,
			&UEFIMMap.MapKey,
			&UEFIMMap.DescriptorSize,
			&UEFIMMap.DescriptorVersion
		);
		if(Status == EFI_BUFFER_TOO_SMALL)									// If it's too small, free the memory, up the size, and try again
		{
			Status = uefi_call_wrapper(
				ST->BootServices->FreePool, 1,
				UEFIMMap.Map
			);
			UEFIMMap.MapSize += sizeof(EFI_MEMORY_DESCRIPTOR) * 16;
		}
		else break;
	}

	uint64_t BestAllocStart = 0, BestNumPages = 0;							// Find the best place to allocate memory from
	for(int x = 0; x < UEFIMMap.MapSize; x++)
	{
		EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)(		// Get next descriptor
			(uint64_t)UEFIMMap.Map + (x * UEFIMMap.DescriptorSize)
		);
		if(Descriptor->Type != EfiConventionalMemory) continue;				// Ignore sections that aren't conventional (allocatable) memory
		if(Descriptor->NumberOfPages <= BestNumPages) continue;				// Ignore any sections that aren't bigger than the current biggest
		BestNumPages = Descriptor->NumberOfPages;
		BestAllocStart = Descriptor->PhysicalStart;
	}

	Print(L"- Memory Map Initialised.\r\n");
}