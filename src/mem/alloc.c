#include <global.h>
#include <types/nums.h>
typedef struct block {
    size_t size;
    struct block *next;
    int free;
} block_t;

#define HEAP_START ((char *)0x100000)
#define HEAP_SIZE (64 * 1024)

static char *heap = HEAP_START;
static block_t *free_list = NULL;

void kheap_init() {
    free_list = (block_t *)heap;
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->next = NULL;
    free_list->free = 1;
}

void *kmalloc(size_t size) {
    block_t *curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
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
