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
#include <io/kio.h>
#include <types/nums.h>
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void MaskAllIrqs(void) {
    Kprintf("MasKing all IRQs\n");
    Outb(PIC1_DATA, 0xFF); // MasK all IRQs on master PIC
    Outb(PIC2_DATA, 0xFF); // MasK all IRQs on slave PIC
}

void UnmaskIrq(u8 irq) {
    Kprintf("UnmasKing IRQ\n");
    u16 port;
    u8 value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq -= 8;
    }

    value = Inb(port) & ~(1 << irq);
    Outb(port, value);
}