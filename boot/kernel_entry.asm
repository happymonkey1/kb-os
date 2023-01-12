global start
[bits 32]

start:
    [extern kernel_entry_point]
    call kernel_entry_point
    jmp $
