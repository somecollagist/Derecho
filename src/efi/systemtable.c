#include "systemtable.h"

#include <bootinfo.h>
#include <efi.h>
#include <screen.h>

uint64_t EFIConfigEntries;
uint64_t EFIConfigAddr;

SystemTableReturnCode TraverseSystemTable()
{
	for(uint64_t ConfigEntry = 0; ConfigEntry < EFIConfigEntries; ConfigEntry++)
	{
		EFI_CONFIGURATION_TABLE ConfigTable = *((EFI_CONFIGURATION_TABLE*)EFIConfigAddr + ConfigEntry);
		SystemTableReturnCode retc;

		printf(COLOUR_PROCESS, " > Traversing GUID: %x8 - %x4 %x4 - %x2 %x2 %x2 %x2 %x2 %x2 %x2 %x2\n",
			ConfigTable.VendorGuid.Data1,
			ConfigTable.VendorGuid.Data2,
			ConfigTable.VendorGuid.Data3,
			ConfigTable.VendorGuid.Data4[0],
			ConfigTable.VendorGuid.Data4[1],
			ConfigTable.VendorGuid.Data4[2],
			ConfigTable.VendorGuid.Data4[3],
			ConfigTable.VendorGuid.Data4[4],
			ConfigTable.VendorGuid.Data4[5],
			ConfigTable.VendorGuid.Data4[6],
			ConfigTable.VendorGuid.Data4[7]
		);

		// This one might be REAL
		// https://github.com/LongSoft/UEFITool/blob/new_engine/common/guids.csv

		#define BranchIfGuid(GUID, Handler)											\
		else if (																	\
			ConfigTable.VendorGuid.Data1	== ((EFI_GUID)GUID).Data1		&&		\
			ConfigTable.VendorGuid.Data2	== ((EFI_GUID)GUID).Data2		&&		\
			ConfigTable.VendorGuid.Data3	== ((EFI_GUID)GUID).Data3		&&		\
			ConfigTable.VendorGuid.Data4[0]	== ((EFI_GUID)GUID).Data4[0]	&&		\
			ConfigTable.VendorGuid.Data4[1]	== ((EFI_GUID)GUID).Data4[1]	&&		\
			ConfigTable.VendorGuid.Data4[2]	== ((EFI_GUID)GUID).Data4[2]	&&		\
			ConfigTable.VendorGuid.Data4[3]	== ((EFI_GUID)GUID).Data4[3]	&&		\
			ConfigTable.VendorGuid.Data4[4]	== ((EFI_GUID)GUID).Data4[4]	&&		\
			ConfigTable.VendorGuid.Data4[5]	== ((EFI_GUID)GUID).Data4[5]	&&		\
			ConfigTable.VendorGuid.Data4[6]	== ((EFI_GUID)GUID).Data4[6]	&&		\
			ConfigTable.VendorGuid.Data4[7]	== ((EFI_GUID)GUID).Data4[7]			\
		) retc = Handler(ConfigTable.VendorTable);

		if(0){}
		BranchIfGuid(ACPI_20_TABLE_GUID, RSDPHandler)
		else retc = SYSTEMTABLE_ERROR_UNKNOWN_GUID;

		// if(retc != SYSTEMTABLE_ERROR_NONE)
		// 	return retc;
	}

	return SYSTEMTABLE_ERROR_NONE;
}

ConfigTableHandler(RSDPHandler)
{
	RSPD2 rspd = *(RSPD2*)Table;
	if(rspd.Signature != 0x2052545020445352) // "RSD PTR ", but in little endian
		return SYSTEMTABLE_ERROR_BAD_RSDP_SIGNATURE;

	return SYSTEMTABLE_ERROR_NONE;
}

