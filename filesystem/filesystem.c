#include "../include/filesystem/filesystem.h"
#include "../include/libc/string.h"

static file_t* root;
static path_t current_dir;

void kb_filesystem_init()
{
    root = init_file();
    root->path.raw_path = "/";

    // initialize current dir
    current_dir = copy_path_struct(root->path);
}

const path_t get_root_path()
{
    return root->path;
}

file_t* get_root_f()
{
    return root;
}

const path_t get_current_dir_path()
{
    return current_dir;
}

// change current directory
void kb_set_current_dir(path_t path)
{
    if (is_valid_path(&path))
        current_dir = path;
}

// check if a directory contains the given child
bool directory_contains_child(file_t dir, const char* child)
{
    // #TODO check that file is directory

    dbll_head_t child_list = dir.children;
    if (child_list.head == NULL)
        return false;

    dbll_node_t* cur = child_list.head;
    while (cur)
    {
        // #TODO there should be a better way to compare files...
        file_t* file = (file_t*)cur->data;
        if (strcmp(file->name, child) == 0)
            return true;

        cur = cur->next;
    }

    return false;
}