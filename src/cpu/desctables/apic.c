#include "desctables.h"

uint8_t		LocalAPICIDs[256] = {0};
uint64_t	LocalAPICptr;
uint64_t	IOAPICptr;

uint8_t		NumProcessors;
uint8_t		NumProcessorsEnabled;

void APICHandler(ACPISDTHeader* header)
{
	printf(COLOUR_STDOUT, " > APIC Descriptor Detected...\n");

	NumProcessors = NumProcessorsEnabled = 0;

	uint8_t* LAPICptr = (uint8_t*)header + 36;
	uint8_t* ptr = (uint8_t*)header + 44;
	uint8_t* endptr = (uint8_t*)((uint32_t)ptr + header->Length);

	/*
		ptr structure:
		Offset	Description
		0		Entry Type
		1		Record Length
		2...	Entry Data (layouts prescribed below)

		Ergo, when assigning an entry type in the switch case, dereference from ptr[2].
	*/

	for(; ptr < endptr; ptr += ptr[1])
	{
		// See this for more help:
		// https://wiki.osdev.org/MADT

		switch(ptr[0])
		{
			case 0:	// Processor Local APIC
				typedef struct {
					uint8_t		ProcessorID;
					uint8_t		APICID;
					uint32_t	Flags;
				} __attribute__((packed)) EntryType0;
				EntryType0* e0 = (EntryType0*)(ptr + 2);

				switch(e0->Flags & (uint32_t)0b11)
				{
					case (uint32_t)0b00:	// Incapable, not enabled
						printf(COLOUR_YELLOW, " ! Processor ID %x could not be enabled.\n", e0->ProcessorID);
						break;

					case (uint32_t)0b10:	// Capable, not enabled
						printf(COLOUR_STDOUT, " - Processor ID %x can be enabled.\n", e0->ProcessorID);
						break;

					case (uint32_t)0b01:	// Capable, lower bit takes precedence
					case (uint32_t)0b11:
						printf(COLOUR_STDOUT, " - Processor ID %x is enabled.\n", e0->ProcessorID);
						LocalAPICIDs[NumProcessors++] = e0->APICID;
						NumProcessorsEnabled++;
						break;

					default:
						printf(COLOUR_STDERR, "0x%x16", e0->Flags);
						break;
				}
				break;

			case 1:	// IO APIC
				typedef struct {
					uint8_t		IOAPICID;
					uint8_t		Reserved;
					uint32_t	IOAPICAddr;
					uint32_t	GlobalSystemInterruptBase;
				} __attribute__((packed)) EntryType1;
				EntryType1* e1 = (EntryType1*)(ptr + 2);

				IOAPICptr = (uint64_t)*((uint32_t*)e1->IOAPICAddr);
				printf(COLOUR_STDOUT, " - IO APIC Address: 0x%xF\n", IOAPICptr);
				break;

			case 2:	// IO APIC Interrupt Source Override
				typedef struct {
					uint8_t		BusSource;
					uint8_t		IRQSource;
					uint32_t	GlobalSystemInterrupt;
					uint16_t	Flags;
				} __attribute__((packed)) EntryType2;
				EntryType2* e2 = (EntryType2*)(ptr + 2);
				break;

			case 3:	// IO APIC Non-Maskable Interrupt Source
				typedef struct {
					uint8_t		NMISource;
					uint8_t		Reserved;
					uint16_t	Flags;
					uint32_t	GlobalSystemInterrupt;
				} __attribute__((packed)) EntryType3;
				EntryType3* e3 = (EntryType3*)(ptr + 2);
				break;

			case 4:	// Local APIC Non-Maskable Interrupts
				typedef struct {
					uint8_t		ProcessorID;
					uint16_t	Flags;
					uint8_t		LINTNumber;
				} __attribute__((packed)) EntryType4;
				EntryType4* e4 = (EntryType4*)(ptr + 2);
				break;

			case 5:	// Local APIC Address Override
				typedef struct {
					uint16_t	Reserved;
					uint64_t	LocalAPICPhysicalAddress;
				} __attribute__((packed)) EntryType5;
				EntryType5* e5 = (EntryType5*)(ptr + 2);

				LAPICptr = *((uint64_t*)e5->LocalAPICPhysicalAddress);
				printf(COLOUR_STDOUT, " - Local APIC Address: 0x%xF\n", LAPICptr);
				break;

			case 9:	// Processor Local x2APIC#
				typedef struct {
					uint16_t	Reserved;
					uint32_t	x2APICID;
					uint32_t	Flags;
					uint32_t	ACPIID;
				} __attribute__((packed)) EntryType9;
				EntryType9* e9 = (EntryType9*)(ptr + 2);
				break;
			
			default:
				printf(COLOUR_STDERR, " ! Unknown Interrupt Controller of type %d\n", ptr[0]);
				break;
		}
	}

	printf(COLOUR_SUCCESS,
		" - MADT parsed successfully\n"
		"   |- Cores available/installed: %d/%d\n"
		"   |- IOAPIC:                    0x%xF\n"
		"   |- LAPIC:                     0x%xF\n"
		"   |- Processor IDs:",
		NumProcessorsEnabled, NumProcessors, IOAPICptr, LAPICptr
	);

	for(int x = 0; x < NumProcessors; x++)
		printf(COLOUR_SUCCESS, " %d", LocalAPICIDs[x]);
	printf(COLOUR_SUCCESS, "\n");
}