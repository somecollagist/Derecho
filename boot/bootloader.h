#pragma once

#include <bootinfo.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <types.h>

#define FailWithMessage(msg) 	\
{								\
	Print(msg);					\
	goto BootloaderFail;		\
}

extern BootInfo BI;

extern EFI_HANDLE IM;
extern EFI_SYSTEM_TABLE* ST;

/// @brief Initialises graphics output protocol
void InitGraphics();

/// @brief Clears all the text on the screen
void ClearGraphics();

/// @brief Reads the memory map from UEFI
void InitMemoryMap();

/// @brief Loads a file on the disk
/// @param Directory Directory of the file, or NULL if root
/// @param Path Name of the file
/// @return Returns a reference to the file
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path);
