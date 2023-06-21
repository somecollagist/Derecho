#include "bootloader.h"

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path)
{
	EFI_FILE* LoadedFile = NULL;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

	uefi_call_wrapper(
		ST->BootServices->HandleProtocol, 3,
		IM,
		&gEfiLoadedImageProtocolGuid,
		(void**)&LoadedImage
	);
	uefi_call_wrapper(
		ST->BootServices->HandleProtocol, 3,
		LoadedImage->DeviceHandle,
		&gEfiSimpleFileSystemProtocolGuid,
		(void**)&FileSystem
	);

	if(Directory == NULL)
		uefi_call_wrapper(
			FileSystem->OpenVolume, 2,
			FileSystem,
			&Directory
		);
	
	EFI_STATUS s = uefi_call_wrapper(
		Directory->Open, 5,
		Directory,
		&LoadedFile,
		Path,
		EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY
	);
	return LoadedFile;
}