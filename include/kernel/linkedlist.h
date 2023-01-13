#pragma once

// linked list implementation

typedef struct ll_node
{
    // data ptr
    void* data;
    // next node ptr
    struct ll_node* next;
} ll_node_t;

// initialize a linked list
void linked_list_init(ll_node_t** head);

// free linked list
void linked_list_free(ll_node_t** head);

// append to the front of a linked list
void linked_list_push_front(ll_node_t* head, void* data);


