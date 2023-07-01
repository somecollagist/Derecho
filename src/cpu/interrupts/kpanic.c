#include "interrupts.h"

#include <screen.h>

void kpanic()
{
	printf(COLOUR_FATAL, "PANIC!!! D:\n");
	asm volatile("cli; hlt");
}