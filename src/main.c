#include <bootinfo.h>
#include <drivers/memory.h>
#include <gdt.h>
#include <screen.h>
#include <string.h>
#include <types.h>

const char* OS          = "Derecho";
const char* VERSION     = "0.0.3";
const char* VERSIONNAME = "Hydrogen";

__attribute__((section("KernelEntrySection")))
/// @brief Kernel main function
/// @param bootinfo Bootinfo passed from the UEFI bootloader
int _start(BootInfo* bootinfo)
{
	UnpackBootInfo(bootinfo);

	InitScreen();
	
	printf(COLOUR_STDOUT, " --- %s v%s %s\n", OS, VERSION, VERSIONNAME);

	InitGDT();
	InitMemory();

	for(;;){}
	return 0;
}
