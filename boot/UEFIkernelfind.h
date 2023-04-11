#pragma once

#include <bootinfo.h>
#include <efi.h>
#include <efilib.h>
#include <elf.h>

extern int(*KernelStart)(BootInfo* bootinfo);

EFI_STATUS LocateKernelStart();