#pragma once

#include <stdbool.h>

#include "../kernel/linkedlist.h"

typedef struct path
{
    // full raw path string
    const char* raw_path;
} path_t;

// check whether path is absolute
bool is_absolute_path(path_t* path);

// check whether path is relative
bool is_relative_path(path_t* path);

// check whether path is valid
bool is_valid_path(path_t* path);

// initialize a path struct
path_t init_path();

// copy path struct
path_t copy_path_struct(const path_t path);

// parse a string into a path struct
path_t parse_str_to_path(const char* raw_path);

