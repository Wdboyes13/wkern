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

// Old Definitions (Kept for clarity)
typedef unsigned char kuint8_t;
typedef unsigned short kuint16_t;
typedef unsigned int kuint32_t;
typedef unsigned long kuintptr_t;
typedef unsigned int size_t;

// New definitions, shorter
/// @brief 32-Bit Unsigned Int
typedef unsigned int u32;
/// @brief 8-Bit Unsigned Int
typedef unsigned char u8;
/// @brief Long Unsigned Int
typedef unsigned long uptr;
/// @brief 16-Bit Unsigned Int
typedef unsigned short u16;