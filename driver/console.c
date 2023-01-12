// serial line protocol https://wiki.osdev.org/Serial_Ports
#define COM1 0x3f8

static int uart; // is there a uart?

void microdelay(unsigned long us) 
{

}

static inline unsigned char inb(unsigned short port)
{
    unsigned char data;
    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data)
{
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

void uartinit(void)
{
    // turn of the FIFO
    outb(COM1+2, 0);

    // 9600 baud, 8 data bits, 1 stop bit, parity off
    // unlock divisor
    outb(COM1 + 3, 0x80);
    outb(COM1, 115200/115200);
    outb(COM1 + 1, 0);
    // lock divisor, 8 data bits
    outb(COM1 + 3, 0x03);
    outb(COM1 + 4, 0);
    // enable recieve interrupts
    outb(COM1 + 1, 0x01);

    // if status is 0xFF, no serial port
    if (inb(COM1+5) == 0xFF)
        return;

    uart = 1;
    
    // acknowledge pre-existing interrupt condition(s);
    inb(COM1+2);
    // enable interrupts
    inb(COM1);
}

void uartputc(int c)
{
    int i;

    if (!uart)
        return;

    for (i = 0; i < 128 && !(inb(COM1+5) & 0x20); ++i)
        microdelay(10);
    
    outb(COM1, c);
}

// print to the console
void serial_print_string(char* str)
{
    int i, c;

    for (i = 0; (c = str[i]) != 0; ++i)
        uartputc(c);
}