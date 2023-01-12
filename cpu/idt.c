#include "../include/cpu/idt.h"
#include "../include/driver/console.h" // serial driver

#include "../include/cpu/registers.h"

// macros to get low and high bits from a 32 bit value
#define LOW_16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES] = { 0 };
idt_register_t idt_register = { 0 };

// register a handler for interrupt n
void set_idt_gate(int n, uint32_t handler)
{
    idt[n].low_offset = LOW_16(handler);
    idt[n].selector = 0x08;
    idt[n].always0 = 0;
    // 0x8E = 1001110
    // P    = 1
    // DPL  = 00
    // 0    = 0
    // D    = 1
    // Type = 110
    idt[n].flags = 0x8E;
    idt[n].high_offset = HIGH_16(handler);

    serial_print_string("idt gate set.\n");
}

void load_idt()
{
    idt_register.base = (uint32_t)&idt;
    idt_register.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidtl (%0)" : : "r" (&idt_register));
}