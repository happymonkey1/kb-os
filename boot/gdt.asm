; setup minimal GDT
; GDT is a style of memory handling called 'segmentation'
; even though we are using paging instead of segmentation
; we are still required to have a valid GDT
; minimal GDT requires
;   a 'zero entry'
;   a 'code segment'
;   a 'data segment'
align 4
section .rodata
gdt_start:  ; do not remove the labels, they are needed to compute sizes and jumps
    ; the GDT starts with a null 8-bytes
    dd 0x0 ; 4 bytes
    dd 0x0 ; 4 bytes
; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf doc, page 36
gdt_code:
    ; 4gb flat r/w/executable code descriptor
    ; careful layout for the segment descriptor (layout can be found in Intel Developer's Manual, 3.4.5)
    dw 0xffff       ; segment length, bits 0-15
    dw 0x0          ; segment base, bits 0-15
    db 0x0          ; segment base, bits 16-23
    db 0b10011010   ; flags (8 bits)
    db 0b11001111   ; flags (4 bits) + segment length, bits 16-19
    db 0x0          ; segment base, bits 24-31
; GDT for data segment
; base and length are identical to code section
; some flags are changed, again, refer to os-dev.pdf
gdt_data:
    ; 4gb flat r/w data descriptor
    dw 0xffff       ; limit low
    dw 0x0          ; base low
    db 0x0          ; base middle
    db 0b10010010   ; access
    db 0b11001111   ; granularity
    db 0x0          ; base high
gdt_end:
; GDT descriptor
; a special data structure that defines the size of the GDT and has a pointer to it in memory
; we compute the size as the difference between gdt_end and gdt_start labels
gdt_descriptor:
    dw gdt_end - gdt_start - 1      ; size (16 bit), always one less than its true size
    dd gdt_start                    ; address (32 bits)
; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start