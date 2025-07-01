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
void Kprintf(const char *fmt, ...);
void Kputchar(char c);
void Kcfp();
void KputcharBackspace();
void KprintHex(u32 num);
char Kgetkey();
unsigned char ScancodeToAscii(unsigned char scancode);
void Kgetstr(char *str, int length);
void Kflush();
void Outb(u16 port, u8 value);
u8 Inb(u16 port);
void Outw(u16 port, u16 value);
u16 Inw(u16 port);
void Cli();
void Sti();
void Outl(u16 port, u32 val);
u32 Inl(u16 port);