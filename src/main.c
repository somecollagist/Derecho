#include <bootinfo.h>
#include <drivers/memory.h>
#include <screen.h>
#include <string.h>
#include <types.h>

const char* OS          = "Derecho";
const char* VERSION     = "0.0.2";
const char* VERSIONNAME = "Hydrogen";

__attribute__((section("KernelEntrySection")))
/// @brief Kernel main function
/// @param bootinfo Bootinfo passed from the UEFI bootloader
int _start(BootInfo* bootinfo)
{
	UnpackBootInfo(bootinfo);

	InitScreen();
	
	printf(COLOUR_STDOUT, " --- %s v%s %s\n", OS, VERSION, VERSIONNAME);

	InitMemory();

	printf(COLOUR_STDOUT,
		" --- Total Memory: %d KiB\n"
		"  |--- Free:       %d KiB\n"
		"  |--- Used:       %d KiB\n"
		"  |--- Reserved:   %d KiB\n",
		GetTotalMemory()/1024,
		GetFreeMemory()/1024,
		GetUsedMemory()/1024,
		GetReservedMemory()/1024
	);

	for(;;){}
	return 0;
}
