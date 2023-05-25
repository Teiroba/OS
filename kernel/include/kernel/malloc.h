#ifndef MALLOC_H
#define MALLOC_H

#include "sbrk.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

const size_t FREE_LIST_NEXT_OFFSET = sizeof(size_t);
const size_t FREE_LIST_PREVIOUS_OFFSET = 2 * sizeof(size_t);
const size_t NEXT_BLOCK_MASK = ~(sizeof(size_t)-4);
const size_t BLOCK_USED_MASK = 0x1;
const size_t PREVIOUS_BLOCK_USED_MASK = 0x2;
const size_t MIN_BLOCK_SIZE = 4 * sizeof(size_t);
const size_t HEADER_SIZE = 3 * sizeof(size_t);
const size_t BOUNDARY_TAG_SIZE = sizeof(size_t);

#define NB_FREE_LISTS 30
// Heads of the free lists.
// `free_lists[i]` contains the head of the list of free blocks with size
// `sizeof(size_t) * 2^(i + fls(MIN_BLOCK_SIZE)) <= size < sizeof(size_t) * 2^(i + 1 + fls(MIN_BLOCK_SIZE))`
void* free_lists[NB_FREE_LISTS] = { NULL };

void* first_block = NULL;
void* last_block = NULL;

int fls(int x) {
    return x ? sizeof(x) * 8 - __builtin_clz(x) : 0;
}

int max(int x, int y) {
    return x > y ? x : y;
}

void** ptr_to_block_header(void* block) {
    return block;
}

void* block_header(void* block) {
    return *ptr_to_block_header(block);
}

void* next_block(void* block) {
    return (uintptr_t)block_header(block) & NEXT_BLOCK_MASK;
}

void* previous_block(void* block) {
    return *((char*)block - BOUNDARY_TAG_SIZE);
}

size_t block_size(void* block) {
    return (char*)next_block(block) - (char*)block;
}

void make_block_boundary_tag(void* block) {
    void* tail_boundary_ptr = (char*)ptr_to_block_header(block) + block_size(block) - BOUNDARY_TAG_SIZE;
    *(uintptr_t*)tail_boundary_ptr = block;
}

void set_next_block(void* block, void* next_block) {
    *(uintptr_t*)ptr_to_block_header(block) &= ~NEXT_BLOCK_MASK;
    *(uintptr_t*)ptr_to_block_header(block) |= (uintptr_t)next_block & NEXT_BLOCK_MASK;
}

bool block_used(void* block) {
    return (uintptr_t)block_header(block) & BLOCK_USED_MASK;
}

bool previous_block_used(void* block) {
    return (uintptr_t)block_header(block) & PREVIOUS_BLOCK_USED_MASK;
}

void set_previous_block_used(void* block, bool previous_used) {
    *(uintptr_t*)ptr_to_block_header(block) &= ~PREVIOUS_BLOCK_USED_MASK;
    *(uintptr_t*)ptr_to_block_header(block) |= PREVIOUS_BLOCK_USED_MASK * previous_used;
}

void set_block_used(void* block, bool used) {
    *(uintptr_t*)ptr_to_block_header(block) &= ~BLOCK_USED_MASK;
    *(uintptr_t*)ptr_to_block_header(block) |= BLOCK_USED_MASK * used;
    
    if (block != last_block) {
        set_previous_block_used(next_block(block), used);
    }
}

void* free_list_next(void* block) {
    return *((char*)block + FREE_LIST_NEXT_OFFSET);
}

void* free_list_previous(void* block) {
    return *((char*)block + FREE_LIST_PREVIOUS_OFFSET);
}

void set_free_list_next(void* block, void* next) {
    *(void**)((char*)block + FREE_LIST_NEXT_OFFSET) = next;
}

void set_free_list_previous(void* block, void* previous) {
    *(void**)((char*)block + FREE_LIST_PREVIOUS_OFFSET) = previous;
}

size_t free_list_of_size(size_t block_size) {
    return fls(block_size) - fls(MIN_BLOCK_SIZE);
}

size_t allign_size(size_t size) {
    return (size + (sizeof(size_t)-1)) & ~(sizeof(size_t)-1);
}

void* find_free_block(size_t size) {
    for (size_t i = free_list_of_size(size); i < NB_FREE_LISTS; i++) {
        if (free_lists[i]) {
            return free_lists[i];
        }
    }

    return NULL;
}

void free_list_delete(void* block) {
    bool is_head = free_list_previous(block) == NULL;
    if (is_head) {
        free_lists[free_list_of_size(block)] = free_list_next(block);
    }

    void* previous = free_list_previous(block);
    void* next = free_list_next(block);
    if (previous) {
        set_free_list_next(previous, next);
    }
    if (next) {
        set_free_list_previous(next, previous);
    }
}

void free_list_add(void* block) {
    size_t i_free_list = free_list_of_size(block_size(block));
    
    set_free_list_previous(block, NULL);
    set_free_list_next(block, free_lists[i_free_list]);
    if (free_lists[i_free_list]) {
        set_free_list_previous(free_lists[i_free_list], block);
    }
    free_lists[i_free_list] = block;
}

void* malloc(size_t size) {
    size_t size_with_header_and_padding = allign_size(max(size + HEADER_SIZE, MIN_BLOCK_SIZE));
    
    void* free_block = find_free_block(size_with_header_and_padding);

    if (free_block != NULL) {
        free_list_delete(free_block);

        size_t remaining_free_size = block_size(free_block) - size_with_header_and_padding;

        bool split = remaining_free_size >= MIN_BLOCK_SIZE;
        if (!split) {
            set_block_used(free_block, true);
            return (char*)free_block + HEADER_SIZE;
        }
        else {
            void* remaining_free_block = (char*)free_block + size_with_header_and_padding;
            set_next_block(remaining_free_block, next_block(free_block));
            set_next_block(free_block, remaining_free_block);
            set_block_used(free_block, true);
            set_block_used(remaining_free_block, false);
            free_list_add(remaining_free_block);
            make_block_boundary_tag(remaining_free_block);
            return (char*)free_block + HEADER_SIZE;
        }
    }
    else {
        void* free_block_end = sbrk(size_with_header_and_padding);
        void* free_block = (char*)free_block_end - size_with_header_and_padding;
        set_next_block(free_block, NULL);
        set_previous_block_used(free_block, last_block != NULL ? block_used(last_block) : true);
        if (last_block != NULL) {
            set_next_block(last_block, free_block);
        }
        if (last_block == NULL) {
            first_block = free_block;
            last_block = free_block;
        }
        last_block = free_block;
        set_block_used(free_block, true);
        return (char*)free_block + HEADER_SIZE;
    }
}

void free(void* block) {
    void* new_block = block;
    void* new_next_block = next_block(block);

    if (!previous_block_used(block)) {
        free_list_delete(previous_block(block));
        new_block = previous_block(block);
    }

    if (block != last_block && !block_used(next_block(block))) {
        free_list_delete(next_block(block));
        new_next_block = next_block(next_block(block));
    }

    if (new_block == last_block) {
        if (new_block != first_block) {
            last_block = previous_block(new_block);
        }
        else {
            first_block = NULL;
            last_block = NULL;
        }
        size_t new_block_size = (char*)new_next_block - (char*)new_block;
        sbrk(-new_block_size);
    }

    set_next_block(new_block, new_next_block);
    set_block_used(new_block, false);
    set_previous_block_used(new_block, true);
    make_block_boundary_tag(new_block);

    free_list_add(new_block);
}

#endif