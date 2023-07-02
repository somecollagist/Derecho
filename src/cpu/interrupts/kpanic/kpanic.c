#include "kpanic.h"

#include <bootinfo.h>
#include <screen.h>

#define COLOUR_PANIC_BACK COLOUR_CRIMSON
#define COLOUR_PANIC_FORE COLOUR_WHITE

#define MAXFRAMES 16

extern char kpart[];

void kpanic(InterruptStackFrame* frame)
{
	StackFrame* stack;
	asm volatile("movq %%rbp, %0" : "=g"(stack) : : "memory");
	asm volatile("cli");

	InitScreen();
	for(int y = 0; y < UEFIGraphics.VerticalResolution; y++)
		for(int x = 0; x < UEFIGraphics.HorizontalResolution; x++)
			PlotPixel(x, y, COLOUR_PANIC_BACK);

	InitScreen();
	
	PlotString(kpart, COLOUR_PANIC_FORE);

	printf(COLOUR_PANIC_FORE, "Exception message: %s\n\n", IntMsgs[frame->int_num]);

	// Print out registers here, I noticed an error so this is a to-do
	printf(COLOUR_PANIC_FORE, "Registers:\n");
	printf(COLOUR_PANIC_FORE, "  cs:  0x%xF  ss:  0x%xF  rflags: 0x%xF\n", frame->cs, frame->ss, frame->rflags);
	printf(COLOUR_PANIC_FORE, "\n");
	printf(COLOUR_PANIC_FORE, "  rax: 0x%xF  rbx: 0x%xF\n", frame->rax, frame->rbx);
	printf(COLOUR_PANIC_FORE, "  rcx: 0x%xF  rdx: 0x%xF\n", frame->rcx, frame->rdx);
	printf(COLOUR_PANIC_FORE, "  rbp: 0x%xF  rsp: 0x%xF\n", frame->rbp, frame->rsp);
	printf(COLOUR_PANIC_FORE, "  rdi: 0x%xF  rsi: 0x%xF\n", frame->rdi, frame->rsi);
	printf(COLOUR_PANIC_FORE, "\n");
	printf(COLOUR_PANIC_FORE, "  r8:  0x%xF  r9:  0x%xF\n", frame->r8, frame->r9);
	printf(COLOUR_PANIC_FORE, "  r10: 0x%xF  r11: 0x%xF\n", frame->r10, frame->r11);
	printf(COLOUR_PANIC_FORE, "  r12: 0x%xF  r13: 0x%xF\n", frame->r12, frame->r13);
	printf(COLOUR_PANIC_FORE, "  r14: 0x%xF  r15: 0x%xF\n", frame->r14, frame->r15);
	printf(COLOUR_PANIC_FORE, "\n");
	printf(COLOUR_PANIC_FORE, "Stack Trace:\n");

	for(int s = 0; s < MAXFRAMES; s++)
	{
		printf(COLOUR_PANIC_FORE, "  0x%xF\n", stack->rip);
		stack = stack->rbp;
		if((uint64_t)stack == 0)
			goto halt;
	}

	halt:
	asm volatile("hlt");

	for(;;){}
}