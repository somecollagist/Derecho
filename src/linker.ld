OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

/* The below sections were aligned by 0x1000 but this seemed to break the OS - might be worth trying to figure out why/fixing this? */

SECTIONS
{
	_KernelStart = .;
	KernelEntrySection :
	{
		*(KernelEntrySection)
	}
	.text :
	{
		*(.text)
	}
	.data :
	{
		*(.data)
	}
	.rodata :
	{
		*(.rodata)
	}
	.bss :
	{
		*(COMMON)
		*(.bss)
	}
	_KernelEnd = .;
}
