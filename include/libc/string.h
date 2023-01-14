#pragma once

#include <stddef.h>
#include <stdbool.h>

// ===================
// internal kernel use
// ===================

// integer to stack allocated string
// mutates str_out
void kb_int_to_str(int num, char str_out[]);

// reverse a string
// mutates str_out
void kb_reverse_str(char str_out[]);

// append to a string
// does ZERO out of bounds checking
void kb_str_append(char* src, char c);

// =====
// c api
// =====

// compute the len of a string
size_t strlen(const char* str);

// check if two strings are equivalent
int strcmp(const char* s1, const char* s2);

// get substring from m to n-1
char* substr(const char* src, int m, int n);

// count number of occurences of char c in str
size_t strcount(const char* src, char c);