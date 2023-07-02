#include "interrupts.h"

#include <screen.h>

extern void ISRHandler(uint64_t number, uint64_t rsp)
{
	switch(number)
	{
		default:
			kpanic((InterruptStackFrame*)rsp);
	}
}