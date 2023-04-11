#include "main.h"

#include <bootinfo.h>
#include <UEFIgraphics.h>
#include <UEFIkernelfind.h>
#include <UEFImemory.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>

BootInfo BI;

uint16_t c;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);
	uefi_call_wrapper(									// Clear the screen
		ST->ConOut->ClearScreen, 1,
		ST->ConOut
	);
	Print(L"Derecho EFI Bootloader\r\n");

	ST = SystemTable;
	EFI_STATUS Status;

	// Put initialisation subroutines here
	InitialiseGraphics();
	InitialiseMemoryMap();

	if(LocateKernelStart(ImageHandle) == EFI_ABORTED)	// Load the kernel
		return EFI_ABORTED;

	uefi_call_wrapper(									// Exit boot services (stops computer from restarting)
		ST->BootServices->ExitBootServices, 2,
		ImageHandle,
		BI.UEFIMemoryMap->MapKey
	);

	uefi_call_wrapper(									// Clear the screen again so we don't need to in the kernel
		ST->ConOut->ClearScreen, 1,
		ST->ConOut
	);

	KernelStart(&BI);									// Jump to the kernel

	for(;;)												// Should never reach this anyway
	return EFI_SUCCESS;
}