global _start
[bits 32]

_start:
    [extern kernel_entry_point]
    call kernel_entry_point
    jmp $
