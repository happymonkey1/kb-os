#pragma once

#include "path.h"
#include "file.h"

#include <stdbool.h>

// initialization function for the filesystem
void kb_filesystem_init();

// return a path struct to the root of the vfs
const path_t get_root_path();

// return file struct to the root of the vfs
file_t* get_root_f();

// return a path struct to the current directory
const path_t get_current_dir_path();

// change current directory
void kb_set_current_dir(path_t path);

// check if a directory contains the given child
// returns index of the child
bool directory_contains_child(file_t path, const char* child);