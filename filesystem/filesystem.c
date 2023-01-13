#include "../include/filesystem/filesystem.h"

static path_t root;
static path_t current_dir;

void kb_filesystem_init()
{
    root = init_path();
    root.raw_path = "/";
    current_dir = copy_path_struct(root);
}

const path_t get_root()
{
    return root;
}

const path_t get_current_dir()
{
    return current_dir;
}

// change current directory
void kb_set_current_dir(path_t path)
{
    if (is_valid_path(path))
        current_dir = path;
}