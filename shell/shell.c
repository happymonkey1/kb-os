#include "../include/shell/shell.h"
#include "../include/libc/string.h"
#include "../include/driver/vga_driver.h"

void kb_exec(const char* cmd)
{
    if (strcmp(cmd, "HALT") == 0)
    {
        print_string("stopping kernel.");
        asm volatile("hlt");
    }
    else if (strcmp(cmd, "HELLO") == 0)
    {
        print_string("hi sharon :)");
        print_string("\n> ");
        return;
    }

    print_string("unknown command: ");
    print_string(cmd);
    print_string("\n> ");
}