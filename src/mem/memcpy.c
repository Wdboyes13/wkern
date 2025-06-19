#include <mem/memcpy.h>
void *kmemcpy(void *dest, const void *src, unsigned int count) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (count--) {
        *d++ = *s++;
    }
    return dest;
}