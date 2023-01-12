#include "../include/driver/vga_driver.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDR 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f



void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    // retrieve high bit from register
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    // retrieve low bit from register
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor()
{
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}

void set_char_at_video_memory(char character, int offset)
{
    unsigned char* video_mem = (unsigned char*)VIDEO_ADDR;
    video_mem[offset] = character;
    video_mem[offset + 1] = WHITE_ON_BLACK;
}

void print_string(const char* str)
{
    int offset = get_cursor();
    int i;
    char c;
    for (i = 0; (c = str[i]) != 0; ++i)
    {
        // scrolling if we reach the bottom of the screen
        if (offset >= MAX_ROWS * MAX_COLS * 2)
            offset = scroll_line(offset);

        if (c == '\n')
            offset = move_offset_to_new_line(offset);
        else
        {
            set_char_at_video_memory(c, offset);
            offset += 2;
        }
    }

    set_cursor(offset);
}

void print_new_line()
{
    print_string("\n");
}

int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

int move_offset_to_new_line(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
}

void memory_copy(uint8_t* source, uint8_t* dest, const int num_bytes)
{
    int i;
    for (i = 0; i < num_bytes; ++i)
        *(dest + i) = *(source + i);
}

int scroll_line(int offset)
{
    uint8_t* source = (uint8_t*)(get_offset(0, 1) + VIDEO_ADDR);
    uint8_t* dest = (uint8_t*)(get_offset(0, 0) + VIDEO_ADDR);
    int size = MAX_COLS * (MAX_ROWS - 1) * 2;
    memory_copy(source, dest, size);

    for (int col = 0; col < MAX_COLS; ++col)
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));

    return offset - 2 * MAX_COLS;
}

void clear_screen()
{
    for (int i = 0; i < MAX_COLS * MAX_ROWS; ++i)
        set_char_at_video_memory(' ', i * 2);
    
    set_cursor(get_offset(0, 0));
}