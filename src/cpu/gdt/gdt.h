#pragma once

#include <types.h>

typedef struct {
	uint16_t	LimitLow;
	uint16_t	BaseLow;
	uint8_t		BaseMid;
	uint8_t		Access;
	uint8_t		LimitFlags;
	uint8_t		BaseHigh;
	uint32_t	BaseTop;
	uint32_t	Reserved;
} __attribute__((packed)) TSSEntry;

typedef struct 
{
	uint32_t	Reserved0;
	uint64_t	RSP[3];
	uint64_t	Reserved1;
	uint64_t	IST[7];
	uint32_t	Reserved2;
	uint32_t	Reserved3;
	uint16_t	Reserved4;
	uint16_t	IOMapBase;
} __attribute__((packed)) TSS;

typedef struct {
	uint16_t	Size;
	uint64_t	Offset;
} __attribute__((packed)) GDTDescriptor;

typedef struct {
	uint16_t	LimitLow;
	uint16_t	BaseLow;
	uint8_t		BaseMid;
	uint8_t		Access;
	uint8_t		LimitFlags;
	uint8_t		BaseHigh;
} __attribute__((packed)) GDTEntry;

#define CreateGDTEntry(base, limit, access, flags)				\
(GDTEntry){														\
	(uint16_t)	( ((uint32_t)limit)			& 0xFFFF	),		\
	(uint16_t)	( ((uint64_t)base)			& 0xFFFF	),		\
	(uint8_t)	( ((uint64_t)base >> 16)	& 0xFF		),		\
	(uint8_t)	( ((uint8_t)access)						),		\
	(uint8_t)													\
	(															\
		( ((uint8_t)limit >> 12)	& 0x0F		) |				\
		( ((uint8_t)flags)			& 0xF0		)				\
	),															\
	(uint8_t)	( ((uint64_t)base >> 24)	& 0xFF		)		\
}

typedef struct 
{
	GDTEntry	Null;
	GDTEntry	KernelCode;
	GDTEntry	KernelData;
	TSSEntry	TSS;
} __attribute__((packed)) GDT;

void InitGDT();