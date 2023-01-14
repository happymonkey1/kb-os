#include "../include/filesystem/file.h"
#include "../include/kernel/linkedlist.h"
#include "../include/memory/memory.h"
#include "../include/libc/string.h"

// initialize file struct
file_t* init_file()
{
    file_t* file = (file_t*)malloc(sizeof(file_t));
    file->name = NULL;
    file->flags = 0;
    file->path = init_path();
    file->parent = NULL;
    file->children = doubly_linked_list_init();
    file->sub_dirs = doubly_linked_list_init();
    file->raw_data = NULL;
    file->raw_data_size = 0;

    return file;
}

// add a child to the parent
int add_child(file_t* parent_f, file_t* child_f)
{
    if (parent_f == NULL || child_f == NULL)
        return -1;

    if (contains_child(parent_f, child_f))
        return 0;
    
    doubly_linked_list_push_back(&parent_f->children, child_f);

    child_f->parent = parent_f;
    // #TODO check if file is directory
    doubly_linked_list_push_back(&parent_f->sub_dirs, child_f);

    return 0;
}

// check if a child is already present in the children list of the parent
bool contains_child(const file_t* parent_f, const file_t* child_f)
{
    dbll_head_t child_list = parent_f->children;
    if (child_list.head == NULL)
        return false;

    dbll_node_t* cur = child_list.head;
    while (cur)
    {
        // #TODO there should be a better way to compare files...
        file_t* file = (file_t*)cur->data;
        if (strcmp(file->name, child_f->name) == 0)
            return true;

        cur = cur->next;
    }

    return false;
}

file_t* get_child_by_name(const file_t* parent_f, const char* child_name)
{
    dbll_head_t child_list = parent_f->children;
    if (child_list.head == NULL)
        return NULL;

    dbll_node_t* cur = child_list.head;
    while (cur)
    {
        // #TODO there should be a better way to compare files...
        file_t* file = (file_t*)cur->data;
        if (strcmp(file->name, child_name) == 0)
            return file;

        cur = cur->next;
    }

    return NULL;
}