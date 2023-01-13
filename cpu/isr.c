#include "../include/cpu/isr.h"
#include "../include/cpu/idt.h"
#include "../include/driver/port.h"
#include "../include/driver/vga_driver.h"
#include "../include/driver/console.h" // serial driver
#include "../include/driver/keyboard_driver.h"
#include "../include/kernel/util.h"
#include "../include/libc/string.h"

#define PRIMARY_PIC_COMMAND 0x20
#define PRIMARY_PIC_DATA 0x21
#define SECONDARY_PIC_COMMAND 0xA0
#define SECONDARY_PIC_DATA 0xA1
#define INTIALIZE_COMMAND 0x11

// macros to get low and high bits from a 32 bit value
#define LOW_16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

// modular irq interrupt handlers so we can dynamically add individual handlers while loading the kernel
isr_t interrupt_handlers[256] = { 0 };

char* exception_messages[] = {
    "division by zero",
    "debug",
    "non maskable interrupt",
    "into detected overflow",
    "out of bounds",
    "invalid opcode",
    "no co-processor",

    "double fault",
    "co-processor segment overrun",
    "bad TSS",
    "segment not present",
    "stack fault",
    "general protection fault",
    "page fault",
    "unknown interrupt",

    "co-processor fault",
    "alignment check",
    "machine check",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved"
};

void isr_install()
{
    /* install isrs */
    serial_print_string("starting to install isrs.\n");
    set_idt_gate(0, (uint32_t) isr0);
    set_idt_gate(1, (uint32_t) isr1);
    set_idt_gate(2, (uint32_t) isr2);
    set_idt_gate(3, (uint32_t) isr3);
    set_idt_gate(4, (uint32_t) isr4);
    set_idt_gate(5, (uint32_t) isr5);
    set_idt_gate(6, (uint32_t) isr6);
    set_idt_gate(7, (uint32_t) isr7);
    set_idt_gate(8, (uint32_t) isr8);
    set_idt_gate(9, (uint32_t) isr9);
    set_idt_gate(10, (uint32_t) isr10);
    set_idt_gate(11, (uint32_t) isr11);
    set_idt_gate(12, (uint32_t) isr12);
    set_idt_gate(13, (uint32_t) isr13);
    set_idt_gate(14, (uint32_t) isr14);
    set_idt_gate(15, (uint32_t) isr15);
    set_idt_gate(16, (uint32_t) isr16);
    set_idt_gate(17, (uint32_t) isr17);
    set_idt_gate(18, (uint32_t) isr18);
    set_idt_gate(19, (uint32_t) isr19);
    set_idt_gate(20, (uint32_t) isr20);
    set_idt_gate(21, (uint32_t) isr21);
    set_idt_gate(22, (uint32_t) isr22);
    set_idt_gate(23, (uint32_t) isr23);
    set_idt_gate(24, (uint32_t) isr24);
    set_idt_gate(25, (uint32_t) isr25);
    set_idt_gate(26, (uint32_t) isr26);
    set_idt_gate(27, (uint32_t) isr27);
    set_idt_gate(28, (uint32_t) isr28);
    set_idt_gate(29, (uint32_t) isr29);
    set_idt_gate(30, (uint32_t) isr30);
    set_idt_gate(31, (uint32_t) isr31);
    serial_print_string("finished installing isrs.\n");

    // install dummy handlers for the remaining values
    for (int i = IRQ15 + 1; i < 256; ++i)
        set_idt_gate(i, (uint32_t)isr_dummy);

    micro_delay();

    /* PIC remmaping */

    // 8259 PIC is repsonsible for managing hardware interrupts
    // NOTE: the APIC is the modern, updated standard (should replace code below)...
    
    // this driver uses a cascade of two PICs, where each of them can handle 8 IRQs
    // the secondary chip is connected to the primary through an IRQ, which effectively
    // gives us 15 different IRQs to handle

    // this bios programs the PIC with default values for the first 16 bit real mode
    // in protected mode, these programmed values conflict with the first 32 gates
    // that are reserved for CPU internal interrupts, thus we need to remap the PIC

    // primary PIC ports
    // 0x20     COMMAND
    // 0x21     DATA
    // secondary PIC PORTS
    // 0xA0     COMMAND
    // 0xA1     DATA
    

    // ICW1
    // first, we have to send the initialize command to both PICS
    serial_print_string("starting PIC remapping.\n");
    /*port_byte_out(PRIMARY_PIC_COMMAND, INTIALIZE_COMMAND);
    port_byte_out(SECONDARY_PIC_COMMAND, INTIALIZE_COMMAND);

    // ICW2 (IDT offset)
    // set 0x20 for primary and 0x28 for secondary
    port_byte_out(PRIMARY_PIC_DATA, PRIMARY_PIC_COMMAND);
    port_byte_out(SECONDARY_PIC_DATA, 0x28);

    // ICW3 (wiring between PICs)
    // tell primary PIC to accept IRQs from secondary PIC on IRQ2
    port_byte_out(PRIMARY_PIC_DATA, 0x04);
    port_byte_out(SECONDARY_PIC_DATA, 0x02);

    // ICW4 (mode)
    // set 0x01 to enable 8086 mode
    port_byte_out(PRIMARY_PIC_DATA, 0x01);
    port_byte_out(SECONDARY_PIC_DATA, 0x01);

    // OCW1 (operational command word)
    // finally send the first operational command word to enable all IRQs (no masking)
    port_byte_out(PRIMARY_PIC_DATA, 0x0);
    port_byte_out(SECONDARY_PIC_DATA, 0x0);*/
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
    serial_print_string("finished PIC remapping.\n");

    micro_delay();

    /* setting up IRQ handlers */

    // IRQ ISRs (primary PIC)
    serial_print_string("setting up irq handlers.\n");
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    
    // IRQ ISRs (secondary PIC)
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);
    serial_print_string("finished setting up irq handlers.\n");

    serial_print_string("loading idt.\n");
    load_idt();
    serial_print_string("finished loading idt.\n");
}

void irq_install()
{
    // enable external interrupts
    print_string("starting external interrupts.\n");
    asm volatile("sti");
    print_string("enabled external interrupts.\n");
    serial_print_string("enabled external interrupts.\n");

    // #TODO IRQ0:timer

    // initialize keyboard driver
    // IRQ1:keyboard
    print_string("starting keyboard driver.\n");
    init_keyboard_driver();
    print_string("initialized keyboard driver.\n");
    serial_print_string("initialized keyboard driver.\n");
}

void isr_handler(registers_t* reg)
{
    serial_print_string("received interrupt: ");
    if (reg->int_no < 31)
        print_string(exception_messages[reg->int_no]);
    else
        print_string("unhandled cpu exception no.\n");
    print_new_line();
}

void irq_handler(registers_t* reg)
{
    if (reg == NULL)
        return;

    // secondary EOI
    if (reg->int_no >= 40)
        port_byte_out(SECONDARY_PIC_COMMAND, PRIMARY_PIC_COMMAND);
     
    // primary EOI 
    port_byte_out(PRIMARY_PIC_COMMAND, PRIMARY_PIC_COMMAND); 

    if (interrupt_handlers[reg->int_no] != 0)
    {
        // #TODO clean up this mess when stringf implemented
        serial_print_string("irq handler ");
        // max size is 256, meaning 3 digits + 1 for null terminator
        char s[4];
        kb_int_to_str(reg->int_no, s);
        serial_print_string(s);
        serial_print_string("called.\n");

        isr_t handler = interrupt_handlers[reg->int_no];
        handler(reg);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    // #TODO clean up this mess when stringf implemented
    char n_str[4];
    kb_int_to_str(n, n_str);
    serial_print_string("registered interrupt handler ");
    serial_print_string(n_str);
    serial_print_string(".\n");

    interrupt_handlers[n] = handler;
}