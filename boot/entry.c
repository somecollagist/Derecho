#include "bootloader.h"

BootInfo BI;

#define FailWithMessage(msg) 	\
{								\
	Print(msg);					\
	goto BootloaderFail;		\
}

EFI_INPUT_KEY Key;

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	IM = ImageHandle;
	ST = SystemTable;

	InitializeLib(ImageHandle, SystemTable);
	uefi_call_wrapper(									// Clear the screen
		ST->ConOut->ClearScreen, 1,
		ST->ConOut
	);
	Print(L"Derecho EFI Bootloader\r\n");

	InitGraphics();
	InitMemoryMap();

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf");
	if(Kernel == NULL)
		FailWithMessage(L"! Could not locate kernel :(\r\n")
	else
		Print(L"- Kernel located successfully.\r\n");

	Elf64_Ehdr* EHeader = NULL;
	
	UINTN EHeaderSize = sizeof(Elf64_Ehdr);
	uefi_call_wrapper(
		ST->BootServices->AllocatePool, 3,
		EfiLoaderData,
		EHeaderSize,
		(void**)&EHeader
	);
	uefi_call_wrapper(
		Kernel->SetPosition, 2,
		Kernel,
		0
	);
	uefi_call_wrapper(
		Kernel->Read, 3,
		Kernel,
		&EHeaderSize,
		EHeader
	);

	if (EHeader == 0)
		FailWithMessage(L"! Kernel ELF Header could not be found :(\r\n")
	else if(
		EHeader->e_ident[EI_MAG0]	!= ELFMAG0		||
		EHeader->e_ident[EI_MAG1]	!= ELFMAG1		||
		EHeader->e_ident[EI_MAG2]	!= ELFMAG2		||
		EHeader->e_ident[EI_MAG3]	!= ELFMAG3		||
		EHeader->e_ident[EI_CLASS]	!= ELFCLASS64	||
		EHeader->e_ident[EI_DATA]	!= ELFDATA2LSB	||
		EHeader->e_type				!= ET_EXEC		||
		EHeader->e_machine			!= EM_X86_64	||
		EHeader->e_version			!= EV_CURRENT
	)
		FailWithMessage(L"! Kernel ELF Header is invalid :(\r\n")
	else
		Print(L"- Kernel ELF Header is valid.\r\n");

	Elf64_Phdr* PHeader;
	UINTN PHeaderSize = EHeader->e_phnum * EHeader->e_phentsize;
	uefi_call_wrapper(
		Kernel->SetPosition, 2,
		Kernel,
		EHeader->e_phoff
	);
	uefi_call_wrapper(
		ST->BootServices->AllocatePool, 3,
		EfiLoaderData,
		PHeaderSize,
		(void**)&PHeader
	);
	uefi_call_wrapper(
		Kernel->Read, 3,
		Kernel,
		&PHeaderSize,
		PHeader
	);

	for(
		Elf64_Phdr* phdr = PHeader;
		(char*)phdr < (char*)PHeader + PHeaderSize;
		phdr = (Elf64_Phdr*)((char*)phdr + EHeader->e_phentsize)
	)
	{
		switch(phdr->p_type)
		{
			case PT_LOAD:
				int Pages = (phdr->p_memsz + EFI_PAGE_SIZE - 1) / EFI_PAGE_SIZE;
				Elf64_Addr Segment = phdr->p_paddr;
				uefi_call_wrapper(
					ST->BootServices->AllocatePages, 4,
					AllocateAddress,
					EfiLoaderData,
					Pages,
					&Segment
				);

				UINTN KernelSize = phdr->p_filesz;
				uefi_call_wrapper(
					Kernel->SetPosition, 2,
					Kernel,
					phdr->p_offset
				);
				uefi_call_wrapper(
					Kernel->Read, 3,
					Kernel,
					&KernelSize,
					(void*)Segment
				);
				break;
		}
	}

	Print(L"- Kernel loaded successfully.\r\n");

	__attribute__((sysv_abi))
	int(*KernelStart)(BootInfo*) = (
		(
			__attribute__((sysvabi))
			int (*)(BootInfo*)
		)
		EHeader->e_entry
	);
	Print(L"- Kernel entry point found at 0x%x.\r\n", EHeader->e_entry);

	uefi_call_wrapper(
		ST->BootServices->ExitBootServices, 2,
		ImageHandle,
		BI.UEFIMemoryMap->MapKey
	);
	
	ClearGraphics();

	KernelStart(&BI);

	Print(L"Derecho terminal state, just keep spinning...");
	for(;;);
	return EFI_SUCCESS;

	BootloaderFail:
		Print(L"\r\nPress ESC to continue to EFI shell...\r\n");

		UINTN KeyEvent = 0;
		do
		{
			uefi_call_wrapper(
				ST->ConIn->Reset, 2,
				ST->ConIn,
				FALSE
			);
			uefi_call_wrapper(
				ST->BootServices->WaitForEvent, 3,
				1,
				&ST->ConIn->WaitForKey,
				&KeyEvent
			);
			uefi_call_wrapper(
				ST->ConIn->ReadKeyStroke, 2,
				ST->ConIn,
				&Key
			);
		} while((UINTN)Key.ScanCode != SCAN_ESC);

		return EFI_ABORTED;
}