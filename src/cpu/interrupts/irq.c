#include "interrupts.h"

#include <screen.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

typedef void(*IRQHandler)(InterruptStackFrame* frame);

IRQHandler IRQs[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void IRQInstallHandler(uint8_t irq, void(*handler)(InterruptStackFrame* frame))
{
	IRQs[irq] = (void*)handler;
}

void IRQUninstallHandler(uint8_t irq)
{
	IRQs[irq] = 0;
}

extern void IRQHandlerMaster(uint64_t number, uint64_t rsp)
{
	if(IRQs[number-32])
	{
		void (*handler)(InterruptStackFrame* frame);
		handler = (void(*)(InterruptStackFrame*))IRQs[number-32];
		handler((InterruptStackFrame*)rsp);
	}
	AcknowledgePIC(number-32);
}

void InitIRQ()
{
	SetIDTDescriptor(32,	irq0,	0x8E);
	SetIDTDescriptor(33,	irq1,	0x8E);
	SetIDTDescriptor(34,	irq2,	0x8E);
	SetIDTDescriptor(35,	irq3,	0x8E);
	SetIDTDescriptor(36,	irq4,	0x8E);
	SetIDTDescriptor(37,	irq5,	0x8E);
	SetIDTDescriptor(38,	irq6,	0x8E);
	SetIDTDescriptor(39,	irq7,	0x8E);
	SetIDTDescriptor(40,	irq8,	0x8E);
	SetIDTDescriptor(41,	irq9,	0x8E);
	SetIDTDescriptor(42,	irq10,	0x8E);
	SetIDTDescriptor(43,	irq11,	0x8E);
	SetIDTDescriptor(44,	irq12,	0x8E);
	SetIDTDescriptor(45,	irq13,	0x8E);
	SetIDTDescriptor(46,	irq14,	0x8E);
	SetIDTDescriptor(47,	irq15,	0x8E);

	// IRQInstallHandler(1, IRQHandlerMaster);

	printf(COLOUR_SUCCESS, " - IRQ Loaded.\n");
}