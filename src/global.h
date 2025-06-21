#pragma once

#include <types/bool.h>
#include <types/nums.h>
static volatile char *vmem = (volatile char *)0xb8000; // x86 vmem address
extern int row;
extern int col;
extern bool capson;

#define ATTR(x) __attribute__((x))
#define PKG ATTR(packed)

extern char name[20];

#define NULL ((void *)0)