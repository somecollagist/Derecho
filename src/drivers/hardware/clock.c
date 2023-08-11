#include "hwdrivers.h"

#include <cpu/cpu.h>
#include <screen.h>

DateTime CurrentDateTime;

static inline uint8_t inb_cmos(uint8_t port)
{
	outb(0x70, port);
	return inb(0x71);
}

void GetCurrentTime()
{
	CurrentDateTime = (DateTime){
		.Seconds	= inb_cmos(0x00),
		.Minutes	= inb_cmos(0x02),
		.Hours		= inb_cmos(0x04),
		.Day		= inb_cmos(0x07),
		.Month		= inb_cmos(0x08),
		.Year		= inb_cmos(0x32)*100 + inb_cmos(0x09)
	};

	uint8_t CMOSStateRegisterB = inb_cmos(0x0B);	// Contains extra date to use in time management
	if(!(CMOSStateRegisterB & 0x04))
	{
		// Convert BCD to binary
		CurrentDateTime = (DateTime){
			.Seconds	= (CurrentDateTime.Seconds	& 0x0F)	+ ((CurrentDateTime.Seconds	/ 16) * 10),
			.Minutes	= (CurrentDateTime.Minutes	& 0x0F)	+ ((CurrentDateTime.Minutes	/ 16) * 10),
			.Hours		= ((CurrentDateTime.Hours	& 0x0F)	+ (((CurrentDateTime.Hours & 0x70)	/ 16) * 10)) | (CurrentDateTime.Hours & 0x80),
			.Day		= (CurrentDateTime.Day		& 0x0F)	+ ((CurrentDateTime.Day		/ 16) * 10),
			.Month		= (CurrentDateTime.Month	& 0x0F)	+ ((CurrentDateTime.Month	/ 16) * 10),
			.Year		= (CurrentDateTime.Year		& 0x0F)	+ ((CurrentDateTime.Year	/ 16) * 10),
		};
	}
	if(!(CMOSStateRegisterB & 0x02) && (CurrentDateTime.Hours & 0x80))
		// Convert to 24 hour time
		CurrentDateTime.Hours = (CurrentDateTime.Hours & 0x7F) + 12;

	CurrentDateTime.Hours %= 24;
}