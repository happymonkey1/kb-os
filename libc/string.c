#include "../include/libc/string.h"
#include "../include/memory/memory.h"

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

size_t strlen(const char* str)
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

// get substring from m to n-1
char* substr(const char* src, int m, int n)
{
    size_t dest_len = n - m;

    char* dest_str = (char*)malloc(dest_len + 1);

    for (int i = m; i < n && (*(src + i) != 0); ++i)
    {
        *dest_str = *(src + i);
        ++dest_str;
    }

    *dest_str = 0;

    return dest_str - dest_len;
}

// count number of occurences of char c in str
size_t strcount(const char* src, char c)
{
    size_t str_len = strlen(src);
    size_t count = 0;
    for (size_t i = 0; i < str_len; ++i)
        if (src[i] == c)
            count++;
    
    return count;
}