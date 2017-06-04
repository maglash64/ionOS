ENTRY(start)

SECTIONS
{
	. = 0x00100000;

	.text BLOCK(16K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(16K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(16K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(16K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
