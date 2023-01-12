#pragma once

// get data from sepcified port
unsigned char port_byte_in(unsigned short port);
// send data to specified port
void port_byte_out(unsigned short port, unsigned char data);