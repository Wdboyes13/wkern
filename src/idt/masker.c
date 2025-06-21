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
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void mask_all_irqs(void) {
    kprintf("Masking all IRQs\n");
    outb(PIC1_DATA, 0xFF); // Mask all IRQs on master PIC
    outb(PIC2_DATA, 0xFF); // Mask all IRQs on slave PIC
}

void unmask_irq(u8 irq) {
    kprintf("Unmasking IRQ\n");
    u16 port;
    u8 value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}