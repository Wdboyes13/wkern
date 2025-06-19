#include <mem/kmem.h>
void kmemset(void *ptr, unsigned char value, unsigned int num) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned int i = 0; i < num; i++) {
        p[i] = value;
    }
}