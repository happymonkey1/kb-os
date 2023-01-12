#include "../include/driver/port.h"

// maps port to dx register, executes in al, dx
// then stores the value of the al register in data c var 
unsigned char port_byte_in(unsigned short port)
{
    unsigned char data;
    asm volatile("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

// executes out dx, al, mapping port to dx and data to al
void port_byte_out(unsigned short port, unsigned char data)
{
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}