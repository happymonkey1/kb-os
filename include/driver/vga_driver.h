#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H

#include "port.h"

void set_cursor(int offset);
int get_cursor();

void set_char_at_video_memory(char character, int offset);

void print_string(const char* str);
void print_new_line();

int get_row_from_offset(int offset);
int get_offset(int col, int row);
int move_offset_to_new_line(int offset);

// copy num_bytes bytes from source to dest
void memory_copy(const char* source, char* dest, const int num_bytes);

int scroll_line(int offset);
void clear_screen();

#endif