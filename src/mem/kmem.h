#pragma once

#include <types/nums.h>

int kmemcmp(const void *a, const void *b, size_t n);
void *kmemcpy(void *dest, const void *src, unsigned int count);
void kmemset(void *ptr, unsigned char value, unsigned int num);