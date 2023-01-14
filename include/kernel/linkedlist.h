#pragma once

#include <stddef.h>
#include <stdbool.h>

// ==========================
// linked list implementation
// ==========================

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

// ==========================


// =================================
// doubly linked list implementation
// =================================

typedef struct dbll_node
{
    // data ptr
    void* data;
    // next node
    struct dbll_node* next;
    // prev node
    struct dbll_node* prev;
} dbll_node_t;

typedef struct dbll_head
{
    dbll_node_t* head;
    size_t size;
} dbll_head_t;

// initialize doubly linked list
dbll_head_t doubly_linked_list_init();

// free doubly linked list
void doubly_linked_list_free(dbll_head_t dbll_head);

// push to front of doubly linked list
// data must be allocated on heap
void doubly_linked_list_push_front(dbll_head_t dbll_head, void* data);

// push to back of doubly linked list
// data must be allocated on heap
void doubly_linked_list_push_back(dbll_head_t dbll_head, void* data);

// return pointer to node at a given index
dbll_node_t* dbll_get_node_at_index(dbll_head_t dbll_head, size_t index);

// return a pointer to an object within the list
// compares pointer addresses
dbll_node_t* dbll_find(dbll_head_t dbll_head, const void* data);

// =================================
