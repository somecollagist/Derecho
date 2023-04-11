#include "graphics.h"

#include <types.h>
#include <efi.h>
#include <efilib.h>

const uint64_t MaxResolution = 1920*1080;

UEFIGraphics UEFIGraph;

void InitialiseGraphics()
{
	EFI_GUID GOPGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;

	uefi_call_wrapper(ST->BootServices->LocateProtocol, 3, &GOPGuid, NULL, (void**)&GOP);
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* GOPInfo;
	uint64_t SizeOfInfo, NumModes, NativeMode;
	EFI_STATUS Status;

	Status = uefi_call_wrapper(GOP->QueryMode, 4, GOP, GOP->Mode==NULL?0:GOP->Mode->Mode, &SizeOfInfo, &GOPInfo);
	if(Status == EFI_NOT_STARTED)
		Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, 0);
	if(!EFI_ERROR(Status))
	{
		NativeMode = GOP->Mode->Mode;
		NumModes = GOP->Mode->MaxMode;
		UEFIGraph.FrameBufferBase		= GOP->Mode->FrameBufferBase;
		UEFIGraph.FrameBufferSize		= GOP->Mode->FrameBufferSize;
		UEFIGraph.HorizontalResolution	= GOP->Mode->Info->HorizontalResolution;
		UEFIGraph.VerticalResolution	= GOP->Mode->Info->VerticalResolution;
		UEFIGraph.PixelsPerScanLine		= GOP->Mode->Info->PixelsPerScanLine;
		UEFIGraph.PixelInformation		= GOP->Mode->Info->PixelInformation;
	}

	uint64_t Ratio = 0, Resolution = 0, BestMode = 0, HorRes, VerRes, ModeRatio, ModeResolution;
	for(uint16_t x = 0; x < NumModes; x++)
	{
		uefi_call_wrapper(GOP->QueryMode, 4, GOP, x, &SizeOfInfo, &GOPInfo);
		HorRes = GOPInfo->HorizontalResolution;
		VerRes = GOPInfo->VerticalResolution;
		ModeRatio = (HorRes<<7)/VerRes;
		ModeResolution = HorRes * VerRes;
		if(ModeResolution <= MaxResolution && ModeResolution > Resolution && ModeRatio >= Ratio)
		{
			Ratio = ModeRatio;
			Resolution = ModeResolution;
			BestMode = x;
		}
	}

	Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, BestMode);
	if(!EFI_ERROR(Status))
	{
		UEFIGraph.FrameBufferBase		= GOP->Mode->FrameBufferBase;
		UEFIGraph.FrameBufferSize		= GOP->Mode->FrameBufferSize;
		UEFIGraph.HorizontalResolution	= GOP->Mode->Info->HorizontalResolution;
		UEFIGraph.VerticalResolution	= GOP->Mode->Info->VerticalResolution;
		UEFIGraph.PixelsPerScanLine		= GOP->Mode->Info->PixelsPerScanLine;
		UEFIGraph.PixelInformation		= GOP->Mode->Info->PixelInformation;
	}

	UEFIGraph.MaxX = UEFIGraph.HorizontalResolution / 8;
	UEFIGraph.MaxY = UEFIGraph.VerticalResolution / 16;
}