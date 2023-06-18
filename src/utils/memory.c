#include "memory.h"

void strcpy(char* src, char* dst)
{
	while(*src != 0)
	{
		*dst = *src;
		src++;
		dst++;
	}
}
