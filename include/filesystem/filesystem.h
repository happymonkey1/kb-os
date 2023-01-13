#pragma once

#include "path.h"
#include "file.h"

// initialization function for the filesystem
void kb_filesystem_init();

// return a path struct to the root of the vfs
const path_t get_root();

// return a path struct to the current directory
const path_t get_current_dir();

// change current directory
void kb_set_current_dir(path_t path);