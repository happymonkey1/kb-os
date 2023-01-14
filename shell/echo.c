#include "../include/shell/echo.h"
#include "../include/driver/vga_driver.h"

#include <stddef.h>

void kb_shell_echo(const char* msg)
{
    if (msg == NULL)
        return;

    print_string(msg);
    print_new_line();
}