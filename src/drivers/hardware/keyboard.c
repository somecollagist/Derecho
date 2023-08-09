#include "hwdrivers.h"

#include <cpu.h>
#include <cpu/interrupts/interrupts.h>
#include <screen.h>

#define PORT_KEYBOARD 0x60

void KeyboardDriver(InterruptStackFrame* frame)
{
	uint8_t scancode = inb(PORT_KEYBOARD);
	printf(COLOUR_STDOUT, "0x%x", scancode);
	if(scancode == 0x0C)
		ACPIPowerOff();
}

void InstallKeyboardHandler()
{
	IRQInstallHandler(1, KeyboardDriver);
}