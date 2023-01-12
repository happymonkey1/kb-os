/* set entry point for this executable */
ENTRY(start)

SECTIONS {
    . = 0x7C00; /* Tells GRUB to load the kernal starting at the 1MB mark */

    .rodata :
    {
        /* ensure that the multiboot header is at the beginning */
        KEEP(*(.multiboot_header))
        *(.rodata .rodata.*)
        . = ALIGN(4K);
    }

    .text :
    {
        *(.text .text.*)
        . = ALIGN(4K);
    }

    .data :
    {
        *(.data .data.*)
        . = ALIGN(4K);
    }

    .bss :
    {
        *(.bss .bss.*)
        . = ALIGN(4K);
    }
}