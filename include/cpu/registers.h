#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct
{
    // data segment selector
    uint32_t ds;
    // general purpose registers pushed by pusha
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;
    // pushed by isr procedure
    uint32_t int_no, err_code;
    // pushed by cpu automatically
    uint32_t eip, cs, eflags, esp, ss;
} registers_t;

typedef struct
{
    // size in bytes
    uint16_t limit;
    // base address of the IDT
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#endif