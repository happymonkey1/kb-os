#include "../include/memory/memory.h"
#include "../include/driver/console.h"

#define NULL_PTR (void*)0
#define DYNAMIC_MEM_MAX_SIZE 4096
#define HEADER_SIZE sizeof(buddy_block_node_t)

static uint8_t buddy_block_alloc_buffer[DYNAMIC_MEM_MAX_SIZE];
static buddy_block_node_t* buddy_block_alloc_head;

// initialize the buddy block allocator
void kb_buddy_block_allocator_init()
{
    buddy_block_alloc_head = (buddy_block_node_t*)buddy_block_alloc_buffer;
    buddy_block_alloc_head->size = DYNAMIC_MEM_MAX_SIZE - HEADER_SIZE;
    buddy_block_alloc_head->next = NULL;
    buddy_block_alloc_head->prev = NULL;
}

// find optimal memory block to "allocate" memory from
void* kb_buddy_block_alloc_find_best_block(buddy_block_node_t* mem_buf, size_t size)
{
    if (mem_buf == NULL)
    {
        serial_print_string("buddy_block_alloc_find_best_block received null memory block?");
        return NULL;
    }

    if (size == 0)
    {
        serial_print_string("buddy_block_alloc_find_best_block received size of zero?");
        return NULL;
    }

    // track the best block we have found
    buddy_block_node_t* best = (buddy_block_node_t*)NULL;
    uint32_t best_block_size = DYNAMIC_MEM_MAX_SIZE + 1;

    // iterate list to find the first, smallest, and unused block
    buddy_block_node_t* cur = mem_buf;
    while (cur)
    {
        // check if the block is better
        bool better_block = !cur->used && cur->size >= (size + HEADER_SIZE) && cur->size <= best_block_size;
        if (better_block)
        {
            best = cur;
            best_block_size = cur->size;
        }

        cur = cur->next;
    }

    return (void*)best;
}

// initialize dynamic memory allocator
void kb_dynamic_memory_init()
{
    // #TODO switch between allocators
    kb_buddy_block_allocator_init();
}

void* malloc(size_t size)
{
    buddy_block_node_t* best_block = kb_buddy_block_alloc_find_best_block((buddy_block_node_t*)buddy_block_alloc_buffer, size);

    if (best_block != NULL)
    {
        // remove size of header
        best_block->size = best_block->size - HEADER_SIZE;

        // split block 
        buddy_block_node_t* allocated = (buddy_block_node_t*)(((uint8_t*)best_block) + HEADER_SIZE + best_block->size);
        
        // update header for split
        allocated->size = size;
        allocated->next = best_block->next;
        allocated->prev = best_block;

        // update doubly linked list in header
        if (best_block->next != NULL)
            best_block->next->prev = allocated;
        best_block->next = allocated;

        // return pointer to directly after the allocated block's header
        return (void*)((uint8_t*)allocated + HEADER_SIZE);
    }

    return NULL;
}

void free(void* mem)
{
    if (mem == NULL)
        return;

    buddy_block_node_t* header = (buddy_block_node_t*)((uint8_t*)mem - HEADER_SIZE);

    // invalid header or memory was not dynamically allocated
    if (header == NULL)
        return;

    header->used = false;

    // try merging blocks
    header = kb_buddy_block_alloc_merge_next_into_current(header);
    header = kb_merge_current_into_previous(header);
}

void* memcpy(void* dest, const void* src, size_t n)
{
    if (src == NULL || dest == NULL || n == 0)
        return NULL;

    // #TODO check headers to make sure we don't go out of bounds
    
    uint8_t* dest_casted = (uint8_t*)dest;
    for (size_t i = 0; i < n; ++i)
        dest_casted[i] = ((const uint8_t*)src)[i];
    
    return (void*)dest_casted;
}

// try merging next block into current block
buddy_block_node_t* kb_buddy_block_alloc_merge_next_into_current(buddy_block_node_t* header)
{
    if (header == NULL)
        return NULL;
    
    buddy_block_node_t* next = header->next;
    if (next != NULL && !next->used)
    {
        // add next block's size into current block
        header->size += next->size;
        header->size += HEADER_SIZE;

        // remove next from linked list
        header->next = next->next;
        if (header->next != NULL)
            header->next->prev = header;
    }

    return header;
}

// try merging current block into previous
buddy_block_node_t* kb_merge_current_into_previous(buddy_block_node_t* header)
{
    if (header == NULL)
        return NULL;

    buddy_block_node_t* prev = header->prev;
    if (prev != NULL && !prev->used)
    {
        // add current block's size to prev
        prev->size += header->size;
        prev->size += HEADER_SIZE;

        // update linked list
        prev->next = header->next;
        if (header->next != NULL)
            header->next->prev = prev;
    }

    return prev;
}