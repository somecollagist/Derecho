#pragma once

#include <screen.h>
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

typedef struct {
	uint8_t		AddressWidth;
	uint8_t		BitWidth;
	uint8_t		BitOffset;
	uint8_t		AccessSize;
	uint64_t	Address;
} __attribute__((packed)) GenericAddressStructure;

void ParseSystemDescriptorTables();

void ACPIHandler(ACPISDTHeader* header);
void FACPHandler(ACPISDTHeader* header);
void DSDTHandler(ACPISDTHeader* header);	// Pointed to by the FADT!