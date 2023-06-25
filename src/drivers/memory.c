#include "memory.h"

#include <bootinfo.h>
#include <screen.h>

#define PAGESIZE 0x1000

// These are linker variables because we need to reserve the kernel space
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

UEFIMemoryMap_t UEFIMemoryMap;

uint64_t TotalMemory = 0;
uint64_t FreeMemory = 0;
uint64_t UsedMemory = 0;
uint64_t ReservedMemory = 0;

uint64_t GetTotalMemory()		{ return TotalMemory; }
uint64_t GetFreeMemory()		{ return FreeMemory; }
uint64_t GetUsedMemory()		{ return UsedMemory; }
uint64_t GetReservedMemory()	{ return ReservedMemory; }

typedef struct
{
	uint64_t Size;
	uint8_t* Buffer;
} MemoryBitmap;

MemoryBitmap MBitmap;

/// @brief Determines if a page is being used
/// @param index Page number
/// @return Returns true if page is being used, otherwise false
bool IsPageUsed(uint64_t index)
{
	uint64_t MacroIndex = index / 8;
	uint8_t MicroIndex = 0b10000000 >> (index % 8);
	return (MBitmap.Buffer[MacroIndex] & MicroIndex) != 0;
}

/// @brief Sets the usage of a page
/// @param index Page number
/// @param value true if page is to be used, otherwise false
void SetPageUsage(uint64_t index, bool value)
{
	uint64_t MacroIndex = index / 8;
	uint8_t MicroIndex = 0b10000000 >> (index % 8);
	uint8_t Mask = MicroIndex;
	if(!value)
		MicroIndex = ~MicroIndex;
		
	MBitmap.Buffer[MacroIndex] ^= Mask;
	if(!value)
		MBitmap.Buffer[MacroIndex] = ~MBitmap.Buffer[MacroIndex];
}

void InitMemory()
{
	TotalMemory = 0;
	FreeMemory = 0;
	UsedMemory = 0;
	ReservedMemory = 0;

	void* LargestFreeMemSeg = nullptr;
	uint64_t LargestFreeMemSegSize = 0;

	// Figure out where best to start allocating memory from
	int MMapDesc;
	for(MMapDesc = 0; MMapDesc < UEFIMemoryMap.MapSize/UEFIMemoryMap.DescriptorSize; MMapDesc++)
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)UEFIMemoryMap.Map + (MMapDesc * UEFIMemoryMap.DescriptorSize));
		TotalMemory += desc->NumberOfPages * PAGESIZE;
		switch(desc->Type)
		{
			case EfiConventionalMemory:
				if(desc->NumberOfPages > LargestFreeMemSegSize)
				{
					LargestFreeMemSeg = desc->PhysicalStart;
					LargestFreeMemSegSize = desc->NumberOfPages;
				}
				break;

			default:
				break;
		}
	}
	LargestFreeMemSegSize *= PAGESIZE;
	FreeMemory = TotalMemory;

	// Use this segment to keep track of all the pages of memory and zero everything (all pages start out as free)
	MBitmap.Size = (TotalMemory / PAGESIZE / 8) + 1;
	MBitmap.Buffer = (uint8_t*)LargestFreeMemSeg;
	for(int BitmapIndex = 0; BitmapIndex < MBitmap.Size; BitmapIndex++)
		*(uint8_t*)(MBitmap.Buffer + BitmapIndex) = 0;

	// Lock and reserve pages as necessary
	LockPages(&MBitmap, MBitmap.Size / PAGESIZE + 1);
	for(MMapDesc = 0; MMapDesc < UEFIMemoryMap.MapSize/UEFIMemoryMap.DescriptorSize; MMapDesc++)
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)UEFIMemoryMap.Map + (MMapDesc * UEFIMemoryMap.DescriptorSize));
		switch(desc->Type)
		{
			case EfiConventionalMemory:
				break;
			
			default:
				ReservePages(desc->PhysicalStart, desc->NumberOfPages);
				break;
		}
	}
	LockPages(&_KernelStart, ((uint64_t)&_KernelEnd-(uint64_t)&_KernelStart) / PAGESIZE + 1);

	printf(COLOUR_STDOUT,
		" - Total Memory: %d KiB\n"
		"   |- Free:      %d KiB\n"
		"   |- Used:      %d KiB\n"
		"   |- Reserved:  %d KiB\n",
		GetTotalMemory()/1024,
		GetFreeMemory()/1024,
		GetUsedMemory()/1024,
		GetReservedMemory()/1024
	);
}

void* RequestPage()
{
	for(int x = 0; x < MBitmap.Size * 8; x++)
	{
		if(IsPageUsed(x))
			continue;
		LockPage((void*)(x * PAGESIZE));
		return (void*)(x * PAGESIZE);
	}

	return nullptr;
}

/// @brief Makes a page available if needed.
/// @param address The address of the page to make available.
/// @return Returns false if page is already free, otherwise true.
bool MakePageAvailable(void* address)
{
	uint64_t index = (uint64_t)address / PAGESIZE;
	if(!IsPageUsed(index))
		return false;
	SetPageUsage(index, false);
	return true;
}

/// @brief Makes a page unavailable if needed.
/// @param address The address of the page to make unavailable.
/// @return Returns false if page is already unavailable, otherwise true.
bool MakePageUnavailable(void* address)
{
	uint64_t index = (uint64_t)address / PAGESIZE;
	if(IsPageUsed(index))
		return false;
	SetPageUsage(index, true);
	return true;
}

void FreePage(void* address)
{
	if(!MakePageAvailable(address))
		return;
	FreeMemory += PAGESIZE;
	UsedMemory -= PAGESIZE;
}

void FreePages(void* address, uint64_t count)
{
	for(int x = 0; x < count; x++)
		FreePage((void*)((uint64_t)address + (x * PAGESIZE)));
}

void LockPage(void* address)
{
	if(!MakePageUnavailable(address))
		return;
	FreeMemory -= PAGESIZE;
	UsedMemory += PAGESIZE;
}

void LockPages(void* address, uint64_t count)
{
	for(int x = 0; x < count; x++)
		LockPage((void*)((uint64_t)address + (x * PAGESIZE)));
}

void UnreservePage(void* address)
{
	if(!MakePageAvailable(address))
		return;
	FreeMemory += PAGESIZE;
	ReservedMemory -= PAGESIZE;
}

void UnreservePages(void* address, uint64_t count)
{
	for(int x = 0; x < count; x++)
		UnreservePage((void*)((uint64_t)address + (x * PAGESIZE)));
}

void ReservePage(void* address)
{
	if(!MakePageUnavailable(address))
		return;
	FreeMemory -= PAGESIZE;
	ReservedMemory += PAGESIZE;
}

void ReservePages(void* address, uint64_t count)
{
	for(int x = 0; x < count; x++)
		ReservePage((void*)((uint64_t)address + (x * PAGESIZE)));
}