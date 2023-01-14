#pragma once

#include "path.h"

#include <stdint.h>
#include <stddef.h>

typedef struct file
{
    // name of file
    const char* name;
    // flags
    uint32_t flags;
    // path struct
    path_t path;
    // contains data for path, parent, children, and subdirectories
    // parent
    struct file* parent;
    // directories on this level
    dbll_head_t children;
    // sub directories
    dbll_head_t sub_dirs;
    // pointer to raw data
    void* raw_data;
    // size of raw data in bytes
    size_t raw_data_size;
} file_t;

// #TODO placeholder
typedef struct 
{
    int placeholder;
} mode_t;

// initialize file struct
file_t* init_file();

// add a child to the parent
int add_child(file_t* parent_f, file_t* child_f);

// check if a child is already present in the children list of the parent
bool contains_child(const file_t* parent_f, const file_t* child_f);

file_t* get_child_by_name(const file_t* parent_f, const char* child_name);
