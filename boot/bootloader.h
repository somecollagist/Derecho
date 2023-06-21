#pragma once

#include <bootinfo.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>

extern BootInfo BI;

extern EFI_HANDLE IM;
extern EFI_SYSTEM_TABLE* ST;

/// @brief Initialises graphics output protocol for Derecho
void InitGraphics();

void ClearGraphics();

void InitMemoryMap();

/// @brief Loads a file on the disk
/// @param Directory Directory of the file, or NULL if root
/// @param Path Name of the file
/// @return Returns a reference to the file
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path);
