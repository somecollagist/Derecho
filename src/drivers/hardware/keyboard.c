#include "hwdrivers.h"

#include <cpu.h>
#include <cpu/interrupts/interrupts.h>
#include <screen.h>

#define PORT_KEYBOARD 0x60

void KeyboardDriver(InterruptStackFrame* frame)
{
	uint8_t scancode = inb(PORT_KEYBOARD);
	switch(scancode)
	{
		case 0x0C:
			ACPIPowerOff();
			break;

		default:
			printf(COLOUR_STDOUT, "0x%x", scancode);
			break;
	}
}

void InstallKeyboardHandler()
{
	IRQInstallHandler(1, KeyboardDriver);
}