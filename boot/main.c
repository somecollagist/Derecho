#include "main.h"

#include <graphics.h>
#include <kernelfind.h>
#include <memory.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);
	uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
	Print(L"Derecho EFI Bootloader\r\n");

	ST = SystemTable;
	EFI_STATUS Status;

	uefi_call_wrapper(ST->BootServices->ExitBootServices, 2, ImageHandle, UEFIMMap.MapKey);
	Print(L"- Exited EFI Boot Services.\r\n");

	InitialiseMemoryMap();
	InitialiseGraphics();

	if(LocateKernelStart(ImageHandle) == EFI_ABORTED)
		return EFI_ABORTED;

	Print(L">> %d\r\n", KernelStart());

	for(;;){}
	return EFI_SUCCESS;
}