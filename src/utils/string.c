#include "string.h"

char Digits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char buffer[64];
char reverse[64];

char* itoa(uint64_t num, uint8_t base)
{
	if(base < 2 || base > 16)
		return ""; // Illegal base, throw an exception?
	int pos, opos, top = 0;

	if(num == 0){
		buffer[0] = '0';
		buffer[1] = '\0';
		return buffer;
	}

	while(num != 0){
		buffer[pos] = Digits[num % base];
		pos++;
		num /= base;
	}
	top = pos--;
	for(opos = 0; opos < top; pos--, opos++){
		reverse[opos] = buffer[pos];
	}
	reverse[opos] = '\0';
	return reverse;
}