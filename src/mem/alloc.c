/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <global.h>
#include <types/nums.h>
typedef struct block {
    size_t size;
    struct block *next;
    int free;
} block_t;

#define HEAP_START ((char *)0x100000)
#define HEAP_SIZE (512 * 1024)

static char *heap = HEAP_START;
static block_t *free_list = NULL;

void kheap_init() {
    free_list = (block_t *)heap;
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->next = NULL;
    free_list->free = 1;
}

void *kmalloc(size_t size, size_t align) {
    size = (size + align - 1) & ~(align - 1);
    block_t *curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size >= size + sizeof(block_t) + 8) {
                // Split the block
                block_t *new_block = (block_t *)((char *)(curr + 1) + size);
                new_block->size = curr->size - size - sizeof(block_t);
                new_block->next = curr->next;
                new_block->free = 1;

                curr->size = size;
                curr->next = new_block;
            }
            curr->free = 0;
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }
    return NULL;
}

void kfree(void *ptr) {
    if (!ptr)
        return;
    block_t *block = (block_t *)ptr - 1;
    block->free = 1;
}
