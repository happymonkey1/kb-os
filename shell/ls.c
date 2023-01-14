#include "../include/shell/ls.h"
#include "../include/driver/vga_driver.h"

// ls command
// posix compliant
// https://pubs.opengroup.org/onlinepubs/009696899/utilities/ls.html
int kb_shell_ls(file_t* file_f)
{
    if (file_f == NULL)
        return -1;
    
    if (!is_valid_path(&(file_f->path)))
        return -1;

    dbll_head_t sub_dir_list = file_f->sub_dirs;
    if (sub_dir_list.head == NULL)
        return -2;
    
    dbll_node_t* cur = sub_dir_list.head;
    while (cur)
    {
        file_t* sub_dir = (file_t*)cur->data;
        // #TODO check that file is valid
        print_string(sub_dir->name);
        print_new_line();

        cur = cur->next;
    }

    return 0;
}