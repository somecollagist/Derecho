#include "UEFIkernelfind.h"

#include <types.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>

int(*KernelStart)();

EFI_FILE_HANDLE GetVolume(EFI_HANDLE ImageHandle)
{
	EFI_LOADED_IMAGE* LoadedImage = NULL;
	EFI_GUID LIPGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	uefi_call_wrapper(
		ST->BootServices->HandleProtocol, 3,
		ImageHandle,
		&LIPGuid,
		(void**)&LoadedImage
	);
	return LibOpenRoot(LoadedImage->DeviceHandle);						// Part of the standard library, gets root of file system
}

uint64_t FileSize(EFI_FILE_HANDLE FileHandle)
{
	uint64_t ret;
	EFI_FILE_INFO* FileInfo = LibFileInfo(FileHandle);					// Part of the standard library, gets file size
	ret = FileInfo->FileSize;
	FreePool(FileInfo);													// Frees allocated memory
	return ret;
}

EFI_STATUS EFIAPI LocateKernelStart(EFI_HANDLE ImageHandle)
{
	EFI_FILE_HANDLE Directory = GetVolume(ImageHandle), Kernel = NULL;
	CHAR16* Filename = L"kernel.elf";

	uefi_call_wrapper(													// Attempts to open the kernel file
		Directory->Open, 5,
		Directory,
		&Kernel,
		Filename,
		EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY
	);
	if(Kernel == NULL)
	{
		Print(L"! Could not load Kernel. :(\r\n");
		return EFI_ABORTED;
	}
	else
	{
		Print(L"- Loaded Kernel File.\r\n");
	}

	Elf64_Ehdr* Header;													// Header from Kernel ELF file
	uint64_t HeaderSize = sizeof(Elf64_Ehdr);
	uefi_call_wrapper(													// Header is located right at the start
		Kernel->SetPosition, 2,
		Kernel,
		0
	);
	uefi_call_wrapper(													// Allocate space for the header
		ST->BootServices->AllocatePool, 3,
		EfiLoaderData,
		HeaderSize,
		(void**)&Header
	);
	uefi_call_wrapper(													// Read header from start of Kernel
		Kernel->Read, 3,
		Kernel,
		&HeaderSize,
		Header
	);
	Print(L"- Loaded Kernel Header.\r\n");

	if(																	// Is the kernel header of the expected type?
		Header->e_ident[EI_CLASS]	!= ELFCLASS64	||
		Header->e_ident[EI_DATA]	!= ELFDATA2LSB	||
		Header->e_type				!= ET_EXEC		||
		Header->e_machine			!= EM_X86_64	||
		Header->e_version			!= EV_CURRENT
	) goto HeaderBad;
	const char *a = &Header->e_ident[EI_MAG0], *b = ELFMAG;
	for(int x = 0; x < SELFMAG; x++)									// Does the kernel have the magic number?
	{
		if(a[x] != b[x]) goto HeaderBad;
	}
	goto HeaderGood;
	HeaderBad:
		Print(L"! Kernel Header is invalid.\r\n");
		return EFI_ABORTED;
	HeaderGood:
		Print(L"- Kernel Header is valid.\r\n");

	Elf64_Phdr* ProgramHeaders;											// Program headers for the kernel, find where the entry point is
	uefi_call_wrapper(													// Move to where they are in the kernel
		Kernel->SetPosition, 2,
		Kernel,
		Header->e_phoff
	);
	uint64_t ProgramHeadersSize = Header->e_phnum * Header->e_phentsize;
	uefi_call_wrapper(													// Allocate space for the program headers
		ST->BootServices->AllocatePool, 3,
		EfiLoaderData,
		ProgramHeadersSize,
		(void**)&ProgramHeaders
	);
	uefi_call_wrapper(													// Read Program headers from kernel
		Kernel->Read, 3,
		Kernel,
		&ProgramHeadersSize,
		ProgramHeaders
	);
	Print(L"- Loaded Kernel Program Headers.\r\n");

	for(																// Loop through each of the program headers
		Elf64_Phdr* ProgramHeader = ProgramHeaders;
		(uint8_t*)ProgramHeader < (uint8_t*)ProgramHeaders + ProgramHeadersSize;
		ProgramHeader = (Elf64_Phdr*)((uint8_t*)ProgramHeader + Header->e_phentsize)
	)
	{
		if(ProgramHeader->p_type == PT_LOAD)							// There ought to only be one of these, find it and leave
		{
			uint64_t Pages = (ProgramHeader->p_memsz + EFI_PAGE_SIZE - 1) / EFI_PAGE_SIZE;
			Elf64_Addr Segment = ProgramHeader->p_paddr;
			uefi_call_wrapper(											// Allocate space for the kernel
				ST->BootServices->AllocatePages, 4,
				AllocateAddress,
				EfiLoaderData,
				Pages,
				Segment
			);
			uefi_call_wrapper(
				Kernel->SetPosition, 2,
				Kernel,
				ProgramHeader->p_offset
			);
			uint64_t KernelSize = ProgramHeader->p_filesz;				// Load kernel into allocated space
			uefi_call_wrapper(
				Kernel->Read, 3,
				Kernel,
				&KernelSize,
				(void*)Segment
			);
			break;
		}
	}
	Print(L"- Kernel Loaded.\r\n");

	int(*KernelStart)(BootInfo*) = (									// Kernel entry point
		(
			__attribute__((sysv_abi)) 
			int(*)(BootInfo*)
		)
		Header->e_entry
	);
	Print(L"- Kernel entry point found at 0x%x.\r\n", Header->e_entry);

	return EFI_SUCCESS;
}