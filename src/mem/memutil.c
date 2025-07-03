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

#include <mem/kmem.h>
/**
 * @brief Set memory to a value
 * @param void* ptr - Pointer to memory which is to be modified
 * @param u8 value - Value to set memory too
 * @param u32 num - Number of bytes to set
 */
void Kmemset(void *ptr, unsigned char value, unsigned int num) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned int i = 0; i < num; i++) {
        p[i] = value;
    }
}
/**
 * @brief Copy memory 
 * @param void* dest - Pointer to memory destination
 * @param void* src - Pointer to source memory
 * @param u32 count - Amount of bytes to be copied
 */
void *Kmemcpy(void *dest, const void *src, unsigned int count) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (count--) {
        *d++ = *s++;
    }
    return dest;
}

/**
 * @brief Compare memory
 *
 * Compares the first `n` bytes of two memory areas.
 *
 * @param a Pointer to the first memory area.
 * @param b Pointer to the second memory area.
 * @param n Number of bytes to compare.
 * @return An integer less than, equal to, or greater than zero if the first differing byte in `a`
 *         is found to be less than, equal to, or greater than the corresponding byte in `b`.
 *         Returns 0 if all `n` bytes are equal.
 */
int Kmemcmp(const void *a, const void *b, size_t n) {
    const unsigned char *p1 = (const unsigned char *)a;
    const unsigned char *p2 = (const unsigned char *)b;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (int)p1[i] - (int)p2[i];
        }
    }
    return 0;
}