#pragma once

#include <efi.h>
#include <efilib.h>
#include <elf.h>

extern int(*KernelStart)();

EFI_STATUS LocateKernelStart();