#pragma once

#include <types/nums.h>

static volatile char *vmem = (volatile char *)0xb8000; // x86 vmem address
static int row = 0;
static int col = 0;

#define ATTR(x) __attribute__((x))
#define PKG ATTR(packed)
