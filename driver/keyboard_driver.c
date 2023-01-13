#include "../include/driver/keyboard_driver.h"
#include "../include/driver/vga_driver.h"
#include "../include/cpu/registers.h"
#include "../include/cpu/isr.h"
#include "../include/driver/console.h" // this is serial console, should probably be renamed
#include "../include/libc/string.h"
#include "../include/shell/shell.h"
#include <stdbool.h> // bool type

// private definitions

// mutates buffer
bool backspace(char buffer[]);

// ===================

const char* keyboard_driver_error_general = "ERR_KEYBOARD_DRIVER_GENERAL";

// ===================
// move to key_codes.h
// ===================

// set to higher number when more scancodes implemeneted
#define SC_MAX 57

#define BACKSPACE 0x0e
#define ENTER 0x1C

const char scan_code_to_char[] = {
    '?', '?', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    '?', '?', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    '?', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`',
    '?', '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', '?', '?',
    '?', ' '
};

// ===================

static char key_buffer[KEY_BUFFER_MAX] = { 0 };
static int key_buffer_ind = 0;

/*
default:
    if (scan_code <= 0x7f)
        print_string("Unknown or unhandled key down.");
    else if (scan_code <= 0x39 + 0x80)
    {
        print_string("Key up ");
        print_letter(scan_code - 0x80);
    }
    else
        print_string("Unknown or unhandled key up.");
    break;
*/

void keyboard_callback(registers_t* registers)
{
    serial_print_string("keyboard callback called!");
    uint8_t scan_code = port_byte_in(0x60);
    
    if (key_buffer_ind == KEY_BUFFER_MAX - 1)
        return;

    if (scan_code > SC_MAX)
    {
        serial_print_string("unrecognized scan code ");
        char scan_code_str[5];
        kb_int_to_str(scan_code, scan_code_str);
        serial_print_string(scan_code_str);
        serial_print_string(".\n");
        return;
    }

    // try backspace
    if (scan_code == BACKSPACE && backspace(key_buffer))
    {
        print_backspace();
        key_buffer_ind--;
        return;
    }

    if (scan_code == ENTER)
    {
        print_new_line();
        // keyboard driver should not be executing commands
        kb_exec(key_buffer);
        key_buffer_ind = 0;
        key_buffer[key_buffer_ind] = 0;
        return;
    }

    // add letter to our buffer
    char code_char = scan_code_to_char[(int)scan_code];
    kb_str_append(key_buffer, code_char);
    key_buffer_ind++;

    char print_letter[2] = { code_char, 0 };

    print_string(print_letter);
}

bool backspace(char buffer[])
{
    size_t buf_len = strlen(buffer);
    if (buf_len > 0) // most likely path
    {
        buffer[buf_len - 1] = 0;
        return true;
    }
    
    return false;
}

void init_keyboard_driver()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
    serial_print_string("registered irq1 handler for keyboard callback\n");
}