#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>

#include "../cpu/registers.h"

// maximum size of the keyboard buffer
#define KEY_BUFFER_MAX 256

void print_letter(uint8_t scan_code);
// public for debugging purposes
void keyboard_callback(registers_t* registers);
void init_keyboard_driver();


#endif