#include <bootinfo.h>
#include <screen.h>
#include <string.h>
#include <types.h>

const char* OS          = "Derecho";
const char* VERSION     = "0.0.2";
const char* VERSIONNAME = "Hydrogen";

__attribute__((section("KernelEntrySection")))
int _start(BootInfo* bootinfo)
{
	UnpackBootInfo(bootinfo);

	InitScreen();
	
	printf(COLOUR_STDOUT, " --- %s v%s %s\n", OS, VERSION, VERSIONNAME);

	for(;;){}
	return 0;
}
