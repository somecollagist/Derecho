#include "bootloader.h"

#include <font.h>
#include <types.h>

const uint64_t MaxResolution = 1920*1080;

UEFIGraphics_t UEFIGraphics;

void InitGraphics()
{
	EFI_GUID GOPGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;

	uefi_call_wrapper(														// Gets GOP
		ST->BootServices->LocateProtocol, 3,
		&GOPGuid,
		NULL,
		(void**)&GOP
	);
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* GOPInfo;
	uint64_t SizeOfInfo, NumModes, NativeMode;
	EFI_STATUS Status;

	Status = uefi_call_wrapper(												// Gets the native mode
		GOP->QueryMode, 4,
		GOP,
		GOP->Mode==NULL?0:GOP->Mode->Mode,
		&SizeOfInfo,
		&GOPInfo
	);
	if(Status == EFI_NOT_STARTED)
		Status = uefi_call_wrapper(											// Sets to native mode if not already there (how this wouldn't be the case I don't know)
			GOP->SetMode, 2,
			GOP,
			0
		);
	if(!EFI_ERROR(Status))
	{
		NativeMode = GOP->Mode->Mode;
		NumModes = GOP->Mode->MaxMode;
		UEFIGraphics.FrameBufferBase		= GOP->Mode->FrameBufferBase;
		UEFIGraphics.FrameBufferSize		= GOP->Mode->FrameBufferSize;
		UEFIGraphics.HorizontalResolution	= GOP->Mode->Info->HorizontalResolution;
		UEFIGraphics.VerticalResolution		= GOP->Mode->Info->VerticalResolution;
		UEFIGraphics.PixelsPerScanLine		= GOP->Mode->Info->PixelsPerScanLine;
		UEFIGraphics.PixelInformation		= GOP->Mode->Info->PixelInformation;
	}

	uint64_t Ratio = 0, Resolution = 0, BestMode = 0, HorRes, VerRes, ModeRatio, ModeResolution;
	for(uint16_t x = 0; x < NumModes; x++)									// Loops for "best" mode - this has the widest ratio and highest resolution
	{
		uefi_call_wrapper(
			GOP->QueryMode, 4,
			GOP,
			x,
			&SizeOfInfo,
			&GOPInfo
		);
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

	Status = uefi_call_wrapper(												// Sets to best mode
		GOP->SetMode, 2,
		GOP,
		BestMode
	);
	if(!EFI_ERROR(Status))
	{
		UEFIGraphics.FrameBufferBase		= GOP->Mode->FrameBufferBase;
		UEFIGraphics.FrameBufferSize		= GOP->Mode->FrameBufferSize;
		UEFIGraphics.HorizontalResolution	= GOP->Mode->Info->HorizontalResolution;
		UEFIGraphics.VerticalResolution		= GOP->Mode->Info->VerticalResolution;
		UEFIGraphics.PixelsPerScanLine		= GOP->Mode->Info->PixelsPerScanLine;
		UEFIGraphics.PixelInformation		= GOP->Mode->Info->PixelInformation;
	}

	UEFIGraphics.MaxX = UEFIGraphics.HorizontalResolution / FontWidth;
	UEFIGraphics.MaxY = UEFIGraphics.VerticalResolution / FontHeight;

	BI.UEFIGraphics = &UEFIGraphics;

	Print(
		L"- Graphical Mode set, resolution is %dx%d\r\n",
		UEFIGraphics.HorizontalResolution,
		UEFIGraphics.VerticalResolution
	);
}

void ClearGraphics()
{
	uefi_call_wrapper(
		ST->ConOut->ClearScreen, 1,
		ST->ConOut
	);
}