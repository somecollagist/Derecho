#include "desctables.h"

#include <cpu/cpu.h>

typedef struct
{
	ACPISDTHeader			Header;
	uint32_t				FirmwareCtrl;
	uint32_t				DSDT;
	uint8_t					Reserved;
	uint8_t					PreferredPowerManagementProfile;
	uint16_t				SCIInterrupt;
	uint32_t				SMICommandPort;
	uint8_t					ACPIEnable;
	uint8_t					ACPIDisable;
	uint8_t					S4BIOSREQ;
	uint8_t					PSTATEControl;
	uint32_t				PM1AEventBlock;
	uint32_t				PM1BEventBlock;
	uint32_t				PM1AControlBlock;
	uint32_t				PM1BControlBlock;
	uint32_t				PM2ControlBlock;
	uint32_t				PMTimerBlock;
	uint32_t				GPE0Block;
	uint32_t				GPE1Block;
	uint8_t					PM1EventLength;
	uint8_t					PM1ControlLength;
	uint8_t					PM2ControlLength;
	uint8_t					PMTimerLength;
	uint8_t					GPE0Length;
	uint8_t					GPE1Length;
	uint8_t					GPE1Base;
	uint8_t					CStateControl;
	uint16_t				WorstC2Latency;
	uint16_t				WorstC3Latency;
	uint16_t				FlushSize;
	uint16_t				FlushStride;
	uint8_t					DutyOffset;
	uint8_t					DutyWidth;
	uint8_t					DayAlarm;
	uint8_t					MonthAlarm;
	uint8_t					Century;
	uint16_t				BootArchitectureFlags;
	uint8_t					Reserved2;
	uint32_t				Flags;
	GenericAddressStructure	ResetReg;
	uint8_t					ResetValue;
	uint8_t					Reserved3[3];
	uint64_t				XFirmwareControl;
	uint64_t				XDSDT;
	GenericAddressStructure	XPM1AEventBlock;
	GenericAddressStructure	XPM1BEventBlock;
	GenericAddressStructure	XPM1AControlBlock;
	GenericAddressStructure	XPM1BControlBlock;
	GenericAddressStructure	XPM2ControlBlock;
	GenericAddressStructure	XPMTimerBlock;
	GenericAddressStructure	XGPE0Block;
	GenericAddressStructure	XGPE1Block;
} __attribute__((packed)) FADT;

void FACPHandler(ACPISDTHeader* header)
{
	printf(COLOUR_STDOUT, " > FACP Descriptor Detected...\n");
	
	FADT* fadt = (FADT*)header;

	/*
	This section will parse the DSDT. It's AML bytecode and one day we may write ourselves an interpreter for it but until then, we use brute force.
	*/

	ACPISDTHeader* dsdt = (ACPISDTHeader*)fadt->XDSDT;
	printf(COLOUR_STDOUT, " > DSDT Descriptor Detected...\n");
	
	uint8_t* offset = (uint8_t*)(dsdt+sizeof(ACPISDTHeader));
	for(; offset < (uint8_t*)(dsdt+dsdt->Length); offset++)
	{
		if(
			*(offset+0) == '_' &&
			*(offset+1) == 'S' &&
			*(offset+2) == '5' &&
			*(offset+3) == '_'
		)
		{
			break;
		}
	}
	if(offset < (uint8_t*)(dsdt+dsdt->Length))
	{
		if((*(offset-1)==0x08) || (*(offset-2)==0x08 && *(offset-1)=='\\') && *(offset+4) == 0x12)	// Validate AML structure
		{
			offset += 5;
			offset += ((*offset &0xC0)>>6) + 2;
			
			if(*offset == 0x0A)
				offset++;
			SLP_TYPa = *offset << 10;
			offset++;

			if(*offset == 0x0A)
				offset++;
			SLP_TYPb = *offset << 10;

			SLP_EN = 1 << 13;
			SCI_EN = 1;
		}
	}

	PM1a_CNT = fadt->PM1AControlBlock;
	PM1b_CNT = fadt->PM1BControlBlock;
	PM1a_EVT = fadt->PM1AEventBlock;
	PM1b_EVT = fadt->PM1BEventBlock;

	outb(fadt->SMICommandPort, fadt->ACPIEnable);
	printf(COLOUR_SUCCESS, " > ACPI Mode Enabled.\n");
}