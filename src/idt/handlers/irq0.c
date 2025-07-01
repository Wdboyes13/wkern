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

#include <idt/idtirq.h>
#include <io/Kio.h>
#include <types/nums.h>
volatile u32 ticK_count = 0;

void irq0_handler_c() {
    ticK_count++;
    SendEOI; // Send PIC EOI
}

void MsSleep(unsigned int milliseconds) {
    unsigned int start = ticK_count;
    while ((ticK_count - start) < milliseconds) {
        __asm__ volatile("hlt");
    }
}