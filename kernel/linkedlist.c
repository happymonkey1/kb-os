#include "../include/kernel/linkedlist.h"
#include "../include/memory/memory.h"

// initialize doubly linked list
dbll_head_t doubly_linked_list_init()
{
    dbll_head_t dbll;
    dbll.head = NULL;//(dbll_node_t*)malloc(sizeof(dbll_node_t));
    dbll.size = 0;

    return dbll;
}

// free doubly linked list
void doubly_linked_list_free(dbll_head_t dbll_head)
{
    if (dbll_head.head == NULL)
        return;

    dbll_node_t* cur = dbll_head.head;
    dbll_node_t* next;
    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }

    dbll_head.head = NULL;
    dbll_head.size = 0;
}

// push to front of doubly linked list
void doubly_linked_list_push_front(dbll_head_t* dbll_head, void* p_data)
{
    // allocate and initialize new node
    // pushing it to front of list
    // and mutating head
    dbll_node_t* new_node = (dbll_node_t*)malloc(sizeof(dbll_node_t));
    new_node->data = p_data;
    if (dbll_head->head == NULL)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        dbll_head->head = new_node;
    }
    else
    {
        new_node->next = dbll_head->head;
        new_node->prev = NULL;
        dbll_head->head->prev = new_node;
        dbll_head->head = new_node;
    }

    // update linked list data
    dbll_head->size++;
}

// push to back of doubly linked list
void doubly_linked_list_push_back(dbll_head_t* dbll_head, void* p_data)
{
    if (dbll_head->head == NULL)
    {
        dbll_node_t* new_node = (dbll_node_t*)malloc(sizeof(dbll_node_t));
        new_node->data = p_data;
        new_node->next = NULL;
        new_node->prev = NULL;

        dbll_head->head = new_node;
        dbll_head->size++;
        return;
    }

    // find last node
    dbll_node_t* cur = dbll_head->head;
    while (cur->next)
        cur = cur->next;

    // allocate and initialize new node
    dbll_node_t* new_node = (dbll_node_t*)malloc(sizeof(dbll_node_t));
    new_node->data = p_data;
    new_node->next = NULL;
    new_node->prev = cur;
    cur->next = new_node;

    // update linked list data
    dbll_head->size++;
}

dbll_node_t* dbll_get_node_at_index(dbll_head_t dbll_head, size_t index)
{
    if (dbll_head.head == NULL)
        return NULL;

    if (index >= dbll_head.size)
        return NULL;

    dbll_node_t* cur = dbll_head.head;
    size_t ind = 0;
    while (cur)
    {
        if (ind == index)
            return cur;
        
        ind++;
        cur = cur->next;
    }

    return NULL;
}

dbll_node_t* dbll_find(dbll_head_t dbll_head, const void* data)
{
    if (dbll_head.head == NULL)
        return NULL;

    dbll_node_t* cur = dbll_head.head;
    while (cur)
    {
        if (cur->data == data)
            return cur;
        
        cur = cur->next;
    }

    return NULL;
}