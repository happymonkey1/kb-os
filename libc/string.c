#include "../include/libc/string.h"

void kb_int_to_str(int num, char str_out[])
{
    int i = 0;

    while (num > 0)
    {
        char c = num % 10 + '0';
        str_out[i++] = c;
        num /= 10;
    }
    
    // null terminator
    str_out[i] = 0;

    kb_reverse_str(str_out);
}

void kb_reverse_str(char str_out[])
{
    size_t str_size = strlen(str_out);
    if (str_size <= 1ull)
        return;
    
    char tmp;
    for (size_t i = 0, j = str_size - 1; i < j; ++i, --j)
    {
        tmp = str_out[i];
        str_out[i] = str_out[j];
        str_out[j] = tmp;
    }
}

size_t strlen(char* str)
{
    size_t size = 0;
    while (str[size] != 0)
        ++size;
    
    return size;
}

void kb_str_append(char* src, char c)
{
    size_t str_size = strlen(src);
    src[str_size] = c;
    src[str_size + 1] = 0;
}

int strcmp(const char* s1, const char* s2)
{
    int i;
    for (i = 0; s1[i] == s2[i]; ++i)
        if (s1[i] == 0)
            return 0;

    return s1[i] - s2[i];
}