#include "../include/filesystem/path.h"
#include "../include/libc/string.h"


// check whether path is absolute
bool is_absolute_path(path_t path)
{
    if (strlen(path.raw_path) == 0)
        return false;
    
    return path.raw_path[0] == '/' ? true : false;
}

// check whether path is relative
bool is_relative_path(path_t path)
{
    if (strlen(path.raw_path) == 0)
        return false;

    return !is_absolute_path(path);
}

// check whether path is valid
bool is_valid_path(path_t path)
{
    if (path.raw_path == NULL)
        return false;
    
    // #TODO
    return true;
}

// initialize a path struct
path_t init_path()
{
    path_t path;
    path.raw_path = NULL;
    path.parent = NULL;
    path.children = doubly_linked_list_init();
    path.sub_dirs = doubly_linked_list_init();
    
    return path;
}

// copy path struct
path_t copy_path_struct(const path_t path)
{
    // is shallow copy fine?
    path_t copy_path;
    copy_path.raw_path = path.raw_path;
    copy_path.parent   = path.parent;
    copy_path.children = path.children;
    copy_path.sub_dirs = path.sub_dirs;

    return copy_path;
}

// parse a string into a path struct
path_t parse_str_to_path(const char* raw_path)
{
    path_t path = init_path();
    path.raw_path = raw_path;

    return path;
}