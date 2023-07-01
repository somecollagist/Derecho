#include "interrupts.h"

#include <screen.h>

__attribute__((aligned(0x10)))
static IDTEntry IDT[IDT_MAX_ENTRIES];
static IDTDescriptor idtDescriptor;

void SetIDTDescriptor(uint8_t vector, void* isr, uint8_t attrs)
{
	IDTEntry* entry = &IDT[vector];

	entry->OffsetLow		= (uint16_t)((uint64_t)isr & 0x000000000000FFFF);
	entry->SegmentSelector	= 0x08;
	entry->IST				= 0;
	entry->Attributes		= attrs;
	entry->OffsetMid		= (uint16_t)(((uint64_t)isr & 0x00000000FFFF0000) >> 16);
	entry->OffsetHigh		= (uint32_t)(((uint64_t)isr & 0xFFFFFFFF00000000) >> 16);
	entry->Reserved			= 0;
}

extern void* ISRTable[];

void InitIDT()
{
	idtDescriptor.Size = (uint16_t)sizeof(IDTEntry) * IDT_MAX_ENTRIES - 1;
	idtDescriptor.Offset = (uint64_t)&IDT;

	for(uint8_t x = 0; x < 48; x++)
	{
		SetIDTDescriptor(x, ISRTable[x], 0x8E);
	}

	asm volatile("lidtq %0" : : "m"(idtDescriptor));
	printf(COLOUR_SUCCESS, " - IDT Loaded.\n");
}