#pragma once

#include <types.h>

extern uint64_t XSDTAddr;

typedef struct {
	uint8_t		Signature[4];
	uint32_t	Length;
	uint8_t		Revision;
	uint8_t		Checksum;
	uint8_t		OEMID[6];
	uint8_t		OEMTableID[8];
	uint32_t	OEMRevision;
	uint32_t	CreatorID;
	uint32_t	CreatorRevision;
} __attribute__((packed)) ACPISDTHeader;

void ParseSystemDescriptorTables();