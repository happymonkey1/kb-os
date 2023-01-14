#include "../include/driver/console.h"
#include "../include/driver/vga_driver.h"
#include "../include/driver/keyboard_driver.h"
#include "../include/cpu/isr.h"
#include "../include/memory/memory.h"
#include "../include/filesystem/filesystem.h"

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

    // enable external interrupts and install irqs
    print_string("setting up interrupt service routines.\n");
    serial_print_string("setting up interrupt service routines.\n");
    irq_install(); 
    serial_print_string("finished setting up interrupt service routines.\n");

    // initialize dynamic memory
    print_string("initializing dynamic memory allocator.\n");
    kb_dynamic_memory_init();
    print_string("finished initializing dynamic memory.\n");

    // initialize vfs
    print_string("initializing filesystem.\n");
    kb_filesystem_init();
    print_string("finished initializing filesystem.\n");

    print_string("welcome to kbOS.\n");
    serial_print_string("welcome to kbOS.\n");

    //path_t root = get_root();
    path_t current_dir = get_current_dir_path();
    print_string(current_dir.raw_path);
    print_string("$ ");

    for (;;)
        continue;

    serial_print_string("kernel clean exit");
    return 0;
}