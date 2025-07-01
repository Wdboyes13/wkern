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
struct GdtEntry gdt[3];
struct GdtPtr gp;

void GdtSetGate(int num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void GdtInstall() {
    gp.limit = (sizeof(struct GdtEntry) * 3) - 1;
    gp.base = (uptr)&gdt;

    GdtSetGate(0, 0, 0, 0, 0);
    GdtSetGate(1, 0, 0xFFFFF, 0x9A, 0xCF);
    GdtSetGate(2, 0, 0xFFFFF, 0x92, 0xCF);

    GdtFlush((uptr)&gp);
}