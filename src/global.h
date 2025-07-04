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

#pragma once

#include <types/bool.h>
#include <types/nums.h>

/// @brief x86 Video Memory Address
__attribute__((unused)) static volatile char *vmem =
    (volatile char *)0xb8000; // x86 vmem address
extern int row;
extern int col;
extern bool capson;
extern bool shift;

/// @brief Apply X attribute
#define ATTR(x) __attribute__((x))
/// @brief Add attribute packed
#define PKG ATTR(packed)

extern char *name;
extern u32 iob;
#define NULL ((void *)0)