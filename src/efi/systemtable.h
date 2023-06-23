#pragma once

#include <types.h>

#define ConfigTableHandler(Name) SystemTableReturnCode Name(void* Table)

typedef enum
{
	SYSTEMTABLE_ERROR_NONE,
	SYSTEMTABLE_ERROR_UNKNOWN_GUID,
	SYSTEMTABLE_ERROR_BAD_RSDP_SIGNATURE = 10,
	SYSTEMTABLE_ERROR_BAD_RSDP_CHECKSUM,
} SystemTableReturnCode;

/// @brief Traverses the system table for extra information for the bootinfo
SystemTableReturnCode TraverseSystemTable();

// Reference ACPI Specification v5, section 5.2.5.3
typedef struct {
	uint64_t	Signature;
	uint8_t		Checksum;
	uint8_t		OEMID[6];
	uint8_t		Revision;
	uint32_t	RSDTAddr;
	uint32_t	Length;
	uint64_t	XSDTAddr;
	uint8_t		ExtendedChecksum;
	uint8_t		ReservedLow;
	uint16_t	ReservedHigh;
} __attribute__((packed)) RSPD2;

ConfigTableHandler(RSDPHandler);