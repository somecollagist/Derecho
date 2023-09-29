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

void memcpy(const void* restrict src, void* restrict dst, size_t count)
{
	const uint8_t* s = src;
	uint8_t* d = dst;

	for(size_t i = 0; i < count; i++)
		*d++ = *s++;
}

void memmove(const void* src, void* dst, size_t count)
{
	const uint8_t* s = src;
	uint8_t* d = dst;

	// If the destination is less than the source, do a regular forward copy
    if(d < s)
        for(size_t i = 0; i < count; i++)
            *d++ = *s++;
	// Otherwise copy the data backwards
    else {
        const uint8_t* lasts = s + (count - 1);
        uint8_t* lastd = d + (count - 1);

        for(size_t i = 0; i < count; i++)
            *lastd-- = *lasts--;
    }
}
