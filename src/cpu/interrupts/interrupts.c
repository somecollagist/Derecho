#include "interrupts.h"

#include <screen.h>

void InitInterrupts()
{
	asm volatile("cli");
	RemapPIC();
	InitIDT();
	InitIRQ();
	asm volatile("sti");
	printf(COLOUR_SUCCESS, " - Interrupts ready.\n");
}