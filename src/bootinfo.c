#include "bootinfo.h"

void UnpackBootInfo(BootInfo* bootinfo)
{
	UEFIGraphics		= *(bootinfo->UEFIGraphics);
	UEFIMemoryMap		= *(bootinfo->UEFIMemoryMap);
	EFIConfigEntries	= bootinfo->EFIConfigEntries;
	EFIConfigAddr		= bootinfo->EFIConfigAddr; 
}
