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
#include <types/nums.h>
void kprintf(const char *msg);
void kputchar(char c);
void kcfp();
void kputchar_backspace();
void kprint_hex(u32 num);
char kgetkey();
unsigned char scancode_to_ascii(unsigned char scancode);
void kgetstr(char *str, int length);
void kflush();
void outb(u16 port, u8 value);
u8 inb(u16 port);
void outw(u16 port, u16 value);
u16 inw(u16 port);
void cli();
void sti();