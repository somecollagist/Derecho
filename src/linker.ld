OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

/* The below sections were aligned by 0x1000 but this seemed to break the OS - might be worth trying to figure out why/fixing this? */

SECTIONS
{
	_KernelStart = .;
	KernelEntrySection : ALIGN(0x1000)
	{
		*(KernelEntrySection)
	}
	.text : ALIGN(0x1000)
	{
		*(.text)
	}
	.data : ALIGN(0x1000)
	{
		*(.data)
	}
	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}
	.bss : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}
	_KernelEnd = .;
}
