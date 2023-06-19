#include "bootinfo.h"

void UnpackBootInfo(BootInfo* bootinfo)
{
	UEFIGraphics = *(bootinfo->UEFIGraphics);
	UEFIMemoryMap = *(bootinfo->UEFIMemoryMap);
}
