#include "string.h"

#include <memory.h>
#include <screen.h>
#include <types.h>
#include <stdarg.h>

char Digits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

char buffer[64];
char reverse[64];

char* itoa(uint64_t num, int base)
{
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

void printf(Colour colour, char* str, ...)
{
  va_list vl;
	int i = 0, j = 0, k = 0;
	char buffer[256] = {0};
	char temporarybuffer[256] = {0};
	char* temp = &temporarybuffer[0];
	va_start(vl, str);
	while(str && str[i])
	{
		if(str[i] == '%')
		{
			i++;
			k = 0;
			switch(str[i])
			{
				case 'c':	// char
					buffer[j] = (char)va_arg(vl, int);
					j++;
					break;

				case 's':	// string / char*
					char* string = (char*)(va_arg(vl, char*));
					while(*string != '\0')
					{
						buffer[j] = *string;
						j++;
						string++;
					}
					break;
				
				case 'd':	// int (base 10)
					strcpy(itoa(va_arg(vl, int), 10), temp);
					while(*temp != '\0')
					{
						buffer[j] = *temp;
						j++;
						temp++;
					}
					break;

				case 'x':	// int (base 16)
					strcpy(itoa(va_arg(vl, int), 16), temp);
					while(*temp != '\0')
					{
						buffer[j] = *temp;
						j++;
						temp++;
					}
					break;
			}
		}
		else
		{
			buffer[j++] = str[i];
		}
		i++;
	}
	PlotString(buffer, colour);
	va_end(vl);
}
