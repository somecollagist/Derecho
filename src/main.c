#include <bootinfo.h>
#include <screen.h>
#include <types.h>

__attribute__((section("KernelEntrySection")))
int _start(BootInfo* bootinfo)
{
	UnpackBootInfo(bootinfo);

	for(;;){}
	return 0;
}