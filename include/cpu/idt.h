#ifndef KB_CPU_IDT_H
#define KB_CPU_IDT_H

#include <stdint.h>

#define KERNEL_GDT_SELECTOR 0x24

typedef struct
{
    // low 16 bits that represents the memory address of the interrupt handler
    // within the respective code segment
    uint16_t low_offset;
    // 16 bit selector of the code segment to jump to when invoking handler
    // this will be kernel code segment
    uint16_t selector;
    // 8 bits of zeros 
    uint8_t always0;
    // flags uses for the interrupt gate
    uint8_t flags;
    // high 16 bits that represents the memory address of the interrupt handler
    // within the respective code segment
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

// set idt gate
void set_idt_gate(int n, uint32_t handler);
// load idt
void load_idt();


#endif