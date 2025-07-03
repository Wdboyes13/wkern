/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
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
typedef struct BlocK {
    size_t size;
    struct BlocK *next;
    int free;
} blocK_t;

#define HEAP_START ((char *)0x100000)
#define HEAP_SIZE (512 * 1024)

static char *heap = HEAP_START;
static blocK_t *free_list = NULL;

/**
 * @brief Initialize Memory Heap
 */
void KheapInit() {
    free_list = (blocK_t *)heap;
    free_list->size = HEAP_SIZE - sizeof(blocK_t);
    free_list->next = NULL;
    free_list->free = 1;
}

/**
 * @brief Allocate memory
 * @param Size - Size of memory to allocate
 * @param Align - Which to align memory to
 * @return Pointer to allocated memory
 */
void *Kmalloc(size_t size, size_t align) {
    size = (size + align - 1) & ~(align - 1);
    blocK_t *curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size >= size + sizeof(blocK_t) + 8) {
                // Split the blocK
                blocK_t *new_blocK = (blocK_t *)((char *)(curr + 1) + size);
                new_blocK->size = curr->size - size - sizeof(blocK_t);
                new_blocK->next = curr->next;
                new_blocK->free = 1;

                curr->size = size;
                curr->next = new_blocK;
            }
            curr->free = 0;
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Free allocated memory
 * @param voifd* - Pointer to allocated memory (from Kmalloc)
 */
void Kfree(void *ptr) {
    if (!ptr) {
        return;
    }
    blocK_t *blocK = (blocK_t *)ptr - 1;
    blocK->free = 1;
}
