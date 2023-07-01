#include "interrupts.h"

#include <screen.h>

extern void ISRHandler(uint64_t number, uint64_t rsp)
{
	printf(COLOUR_ORANGE, " ! Handling exception");
	asm volatile("cli; hlt");
}