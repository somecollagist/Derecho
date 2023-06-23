#include "gdt.h"

#include <screen.h>

GDT DefaultGDT =
(GDT){
	CreateGDTEntry(0x00, 0x00000, 0x00, 0x00),	// Null
	CreateGDTEntry(0x00, 0xFFFFF, 0x9A, 0xA0),	// KernelCode
	CreateGDTEntry(0x00, 0xFFFFF, 0x92, 0xC0),	// KernelData
};

TSS DefaultTSS = 
(TSS){
	0, {}, 0, {}, 0, 0, 0, 0
};

extern void LoadGDT(GDTDescriptor* gdtDescriptor);

void InitGDT()
{
	uint64_t tssaddr = &DefaultTSS;
	uint32_t tsssize = sizeof(TSS);
	DefaultGDT.TSS.LimitLow		= ( ((uint32_t)tsssize)			& 0xFFFF	);
	DefaultGDT.TSS.LimitFlags	= ( ((uint32_t)tsssize >> 12)	& 0x0F		);
	DefaultGDT.TSS.BaseLow		= ( ((uint64_t)tssaddr)			& 0xFFFF	);
	DefaultGDT.TSS.BaseMid		= ( ((uint64_t)tssaddr >> 16)	& 0xFF		);
	DefaultGDT.TSS.BaseHigh		= ( ((uint64_t)tssaddr >> 24)	& 0xFF		);
	DefaultGDT.TSS.BaseTop		= (	((uint64_t)tssaddr >> 32)	& 0xFFFFFFFF);
	DefaultGDT.TSS.Access		= 0x89;
	DefaultGDT.TSS.Reserved		= 0;

	GDTDescriptor GDTDesc;
	GDTDesc.Size				= sizeof(GDT) - 1;
	GDTDesc.Offset				= (uint64_t)&DefaultGDT;
	LoadGDT(&GDTDesc);
	printf(COLOUR_SUCCESS, " --- GDT Loaded.\n");
}