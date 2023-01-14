#include "../include/shell/mkdir.h"
#include "../include/libc/string.h"
#include "../include/driver/vga_driver.h"
#include "../include/memory/memory.h"
#include "../include/filesystem/filesystem.h"

// this is wasting memory
#define MAX_DIRECTORY_COUNT 256
char* directory_name_cache[MAX_DIRECTORY_COUNT];

int kb_shell_mkdir(const char* p_path, mode_t mode)
{
    path_t path = parse_str_to_path(p_path);
    if (!is_valid_path(&path))
        return -1;

    // get number of directories
    size_t dir_level = strcount(path.raw_path, '/');
    if (dir_level == 0)
        return -1;

    if (is_absolute_path(&path))
        dir_level--;
    else
    {
        print_string("relative paths for mkdir not supported yet.");
        return -1;
    }

    if (dir_level >= MAX_DIRECTORY_COUNT)
    {
        print_string("mkdir can only make 256 directories at once.");
        return -1;
    }

    size_t i = is_absolute_path(&path) ? 1 : 0;
    size_t path_len = strlen(path.raw_path);
    size_t p = i;
    size_t count = 0;
    for (; i < path_len + 1; ++i)
    {
        if (path.raw_path[i] == '/' || path.raw_path[i] == 0)
        {
            // #TODO allocates buffer 1 byte too big...
            // get substr copy
            size_t size = i - p;
            char* substr = (char*)malloc(size + 1);
            memcpy(substr, ((const uint8_t*)path.raw_path) + p, size);
            substr[size] = 0;
            p = i + 1;

            directory_name_cache[count++] = substr;
        }
    }
 
    // #TODO this should be based on the current directory if the mkdir path is relative
    file_t* cur = get_root_f();
    for (size_t i = 0; i < count; ++i)
    {
        // check if directory exists
        const char* dir_name = directory_name_cache[i];
        if (directory_contains_child(*cur, dir_name))
            cur = get_child_by_name(cur, dir_name);
        else
        {
            file_t* new_child_dir = init_file();

            // copy dirname to new file entry
            size_t dir_name_len = strlen(dir_name);
            char* substr = (char*)malloc(dir_name_len + 1);
            memcpy(substr, dir_name, dir_name_len);
            substr[dir_name_len] = 0;

            new_child_dir->name = substr;

            add_child(cur, new_child_dir);

            if (i < count - 1)
                cur = new_child_dir;
        }
    }

    // free allocated dir names
    for (size_t i = 0; i < count; ++i)
        free(directory_name_cache[i]);

    return 0;
}