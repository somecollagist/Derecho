#pragma once

#include <types.h>

typedef struct{
	uint8_t		Seconds;
	uint8_t		Minutes;
	uint8_t		Hours;
	uint8_t		Day;
	uint8_t		Month;
	uint16_t	Year;
} DateTime;

extern DateTime CurrentDateTime;

void GetCurrentTime();

void InstallKeyboardHandler();

void InstallHardwareDrivers();