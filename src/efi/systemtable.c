#include "systemtable.h"

#include <bootinfo.h>
#include <cpu/desctables.h>
#include <efi.h>
#include <guids.h>
#include <screen.h>

uint64_t EFIConfigEntries;
uint64_t EFIConfigAddr;

SystemTableReturnCode TraverseSystemTable()
{
	for(int ConfigEntry = 0; ConfigEntry < EFIConfigEntries; ConfigEntry++)
	{
		EFI_CONFIGURATION_TABLE* ConfigTable = ((EFI_CONFIGURATION_TABLE*)EFIConfigAddr + ConfigEntry);
		SystemTableReturnCode retc = SYSTEMTABLE_ERROR_NONE;

		#define BranchIfGuid(GUID, Handler)												\
		else if (																		\
			ConfigTable->VendorGuid.Data1		== ((EFI_GUID)GUID).Data1		&&		\
			ConfigTable->VendorGuid.Data2		== ((EFI_GUID)GUID).Data2		&&		\
			ConfigTable->VendorGuid.Data3		== ((EFI_GUID)GUID).Data3		&&		\
			ConfigTable->VendorGuid.Data4[0]	== ((EFI_GUID)GUID).Data4[0]	&&		\
			ConfigTable->VendorGuid.Data4[1]	== ((EFI_GUID)GUID).Data4[1]	&&		\
			ConfigTable->VendorGuid.Data4[2]	== ((EFI_GUID)GUID).Data4[2]	&&		\
			ConfigTable->VendorGuid.Data4[3]	== ((EFI_GUID)GUID).Data4[3]	&&		\
			ConfigTable->VendorGuid.Data4[4]	== ((EFI_GUID)GUID).Data4[4]	&&		\
			ConfigTable->VendorGuid.Data4[5]	== ((EFI_GUID)GUID).Data4[5]	&&		\
			ConfigTable->VendorGuid.Data4[6]	== ((EFI_GUID)GUID).Data4[6]	&&		\
			ConfigTable->VendorGuid.Data4[7]	== ((EFI_GUID)GUID).Data4[7]			\
		) { printf(COLOUR_SUCCESS, " - Encountered %s, jumping to handler...\n", #GUID); retc = Handler(ConfigTable->VendorTable); }

		if(0){}
		BranchIfGuid(EfiAcpi10TableGUID, RSDP1Handler)
		BranchIfGuid(EfiAcpi20TableGUID, RSDP2Handler)
		else
			retc = SYSTEMTABLE_ERROR_UNKNOWN_GUID;

		if (retc == SYSTEMTABLE_ERROR_UNKNOWN_GUID)
			printf(COLOUR_PROCESS, " - Encountered Unknown GUID: %x8-%x4-%x4-%x2%x2%x2%x2%x2%x2%x2%x2, skipping...\n",
				ConfigTable->VendorGuid.Data1,
				ConfigTable->VendorGuid.Data2,
				ConfigTable->VendorGuid.Data3,
				ConfigTable->VendorGuid.Data4[0],
				ConfigTable->VendorGuid.Data4[1],
				ConfigTable->VendorGuid.Data4[2],
				ConfigTable->VendorGuid.Data4[3],
				ConfigTable->VendorGuid.Data4[4],
				ConfigTable->VendorGuid.Data4[5],
				ConfigTable->VendorGuid.Data4[6],
				ConfigTable->VendorGuid.Data4[7]
			);
		else if(retc != SYSTEMTABLE_ERROR_NONE)
			printf(COLOUR_STDERR, " ! Handler returned error code %d :(\n", retc);
	}

	return SYSTEMTABLE_ERROR_NONE;
}

ConfigTableHandler(NotYetImplementedHandler)
{
	printf(COLOUR_STDERR, " ! Not yet implemented!\n");
	return SYSTEMTABLE_ERROR_NONE;
}

ConfigTableHandler(RSDP1Handler)
{
	printf(COLOUR_STDOUT, "RSDT is not supported in favour of XSDT.\n");
	return SYSTEMTABLE_ERROR_NONE;
}

ConfigTableHandler(RSDP2Handler)
{
	RSPD2 rspd = *(RSPD2*)Table;
	if(rspd.Signature != 0x2052545020445352) // "RSD PTR ", but in little endian
		return SYSTEMTABLE_ERROR_BAD_RSDP_SIGNATURE;
	if(rspd.Revision != 2)
		return SYSTEMTABLE_ERROR_BAD_RSDP_VERSION;
	
	uint8_t Checksum = 0;
	for(uint8_t rspdoffset = 0; rspdoffset < sizeof(RSPD2); rspdoffset++)
		Checksum += *((uint8_t*)&rspd + rspdoffset);
	if(Checksum != 0)
		return SYSTEMTABLE_ERROR_BAD_RSDP_CHECKSUM;

	printf(COLOUR_STDOUT, "XSDT Address: 0x%x\n", rspd.XSDTAddr);
	XSDTAddr = rspd.XSDTAddr;

	return SYSTEMTABLE_ERROR_NONE;
}
