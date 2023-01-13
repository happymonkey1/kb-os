#include "../include/driver/console.h"
#include "../include/driver/vga_driver.h"
#include "../include/driver/keyboard_driver.h"
#include "../include/cpu/isr.h"

int kernel_entry_point()
{
    // intialize console
    uartinit();
    serial_print_string("serial driver initialized.\n");

    // intiailize vga driver
    clear_screen();
    print_string("initialized vga driver.\n");
    serial_print_string("initialized vga driver.\n");

    // initialize interrupt services routine
    isr_install();

    print_string("initializing interrupt service routines.\n");
    // enable external interrupts and install irqs
    irq_install(); 
    serial_print_string("initialized interrupt service routines.\n");

    print_string("welcome to kbOS.\n");
    serial_print_string("welcome to kbOS.\n");

    serial_print_string("testing interrupts.\n");
    // test interrupts
   //asm volatile("int $2");
    //asm volatile("int $3");
    serial_print_string("finished testing interrupts.\n");

    return 0;
}