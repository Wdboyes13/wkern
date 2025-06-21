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

#include <idt/idtirq.h>
#include <io/kio.h>
#include <types/nums.h>
volatile u32 tick_count = 0;

void irq0_handler_c() {
    tick_count++;
    outb(0x20, 0x20); // PIC EOI
}

void ms_sleep(unsigned int ms) {
    unsigned int start = tick_count;
    while ((tick_count - start) < ms) {
        __asm__ volatile("hlt");
    }
}