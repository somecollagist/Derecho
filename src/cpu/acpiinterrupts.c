#include "cpu.h"

#include <screen.h>

void ACPIPowerOff()
{
	printf(COLOUR_STDOUT, " > Powering off...\n");
	outw(PM1a_CNT, SLP_TYPa | SLP_EN);
	printf(COLOUR_FATAL, " ! ACPI Poweroff failed!\n");
}