#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// buddy block memory allocator
// LOL just realized that this memory implementation is not a buddy list...
// #TODO actually implement buddy allocator...
typedef struct buddy_block_node
{
    // size of the block
    uint32_t size;
    // flag for whether the block is used
    bool used;
    // pointer to next block
    struct buddy_block_node* next;
    // pointer to previous block
    struct buddy_block_node* prev;
} buddy_block_node_t;

// initialize the buddy block allocator
void kb_buddy_block_allocator_init();

// find optimal memory block to "allocate" memory from
void* kb_buddy_block_alloc_find_best_block(buddy_block_node_t* mem_buf, size_t size);

// try merging next block into current block
buddy_block_node_t* kb_buddy_block_alloc_merge_next_into_current(buddy_block_node_t*);

// try merging current block into previous
buddy_block_node_t* kb_merge_current_into_previous(buddy_block_node_t*);

// initialize dynamic memory allocator
void kb_dynamic_memory_init();

void* malloc(size_t size);
void free(void* mem);
