section .multiboot_header
header_start:
    dd 0xe85250d6                ; magic number for multiboot spec
    dd 0                         ; protected mode code
    dd header_end - header_start ; header length

    ; checksum
    ; chesum + magic_number + architecture + header_length = 0
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; required end tag
    ; dw defines a word (16 bits on x86_64 arch)
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
