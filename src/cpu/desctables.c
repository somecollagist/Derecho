#include "desctables.h"

#include <screen.h>

uint64_t XSDTAddr = 0;

void ParseSystemDescriptorTables()
{
	printf(COLOUR_PROCESS, " - Parsing System Descriptor Tables...\n");
	if(XSDTAddr == 0)
	{
		printf(COLOUR_FATAL, " ! XSDT Address has not been set!\n");
		return;
	}

	int length = (((ACPISDTHeader*)XSDTAddr)->Length - sizeof(ACPISDTHeader)) / 8;
	typedef struct {
		ACPISDTHeader	Header;
		uint64_t		SDTptrs[length];
	} __attribute__((packed)) XSDT;
	XSDT* xsdt = (XSDT*)XSDTAddr;
	int entries = ((xsdt->Header.Length - sizeof(xsdt->Header)) / 8);
	for(int x = 0; x < entries; x++)
	{
		ACPISDTHeader *Header = (ACPISDTHeader*)xsdt->SDTptrs[x];
		switch((uint32_t)(Header->Signature))
		{
			// Consult ACPI Spec, Section 5.2, tables 5.5 and 5.6for DESCRIPTION_HEADER signatures
			
			default:
				printf(COLOUR_STDERR, " ! Unknown ACPI System Descriptor signature \"%c%c%c%c\"\n",
					Header->Signature[0], Header->Signature[1], Header->Signature[2], Header->Signature[3]);
				break;
		}
	}

	printf(COLOUR_SUCCESS, " - System Descriptor Tables parsed successfully.\n");
}