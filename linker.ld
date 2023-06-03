ENTRY(_start)


SECTIONS
{
    . = 1M;

    .text ALIGN(4K):    /** Align to 4KiB boundary. */
    {
        KEEP(*(.multiboot))     
        *(.text)
        *(.comment)
    }

    .rodata ALIGN(4K):
    {
        *(.rodata)
    }

    .data ALIGN(4K):
    {
        *(.data)
    }

    .bss ALIGN(4K):
    {
        *(.bss)     
        *(COMMON)
    }
}
