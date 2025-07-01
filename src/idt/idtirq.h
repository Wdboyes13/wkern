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

#include <global.h>
#include <types/nums.h> // put includes at the top

#define KERNEL_CODE_SEGMENT 0x08 // Usually your GDT code segment

/*
===================
-- IDT FUNCTIONS --
===================
*/
void SetupIdt(void);
void IdtInit(void);
void IdtSetGate(u8 num, u32 base, u16 sel, u8 flags);
void MaskAllIrqs(void);
void UnmaskIrq(u8 irq);
void DebugPrintIdtEntry(int i);

void AllIdt();

// PIC remap
extern void _picr(void);

/*
==================
---- STRUCTS -----
==================
*/

// IDT Entry structure (packed to prevent compiler padding)
struct IdtEntry {
    u16 base_lo;
    u16 sel; // Kernel code segment selector
    u8 always0;
    u8 flags; // Flags (present, ring level, type)
    u16 base_hi;
} PKG;

struct GdtEntry {
    u16 limit_low;  // Lower 16 bits of limit
    u16 base_low;   // Lower 16 bits of base
    u8 base_middle; // Next 8 bits of base
    u8 access;      // Access flags
    u8 granularity; // Granularity flags + upper 4 bits of limit
    u8 base_high;   // Last 8 bits of base
} PKG;

struct GdtPtr {
    u16 limit;
    u32 base;
} PKG;

struct PKG IdtPtr {
    u16 limit;
    u32 base;
};

/*
===================
-- GDT FUNCTIONS --
===================
*/
void GdtInstall();
extern void GdtFlush(u32);
void GdtSetGate(int num, u32 base, u32 limit, u8 access, u8 gran);

/*
============
-- OTHERS --
============
*/
extern struct IdtPtr idt_ptrn;
#define IDT_ENTRIES 256
extern struct IdtEntry idt[IDT_ENTRIES];
void MsSleep(unsigned int milliseconds);
void PitInit(u32 freq);

// ----- HANDLERS ----- //

void Irq0HandlerC(void);       // C handler called from asm IRQ0 stub
extern void Irq0Handler(void); // Assembly IRQ0 stub (global in asm)
extern void GoodHandler(void);
extern void Irq1Handler(void);
void VirtnetIrqHandler();
extern void VirtnetHandler(void);

// MACROS
#define PIT_FREQ 119
#define SendEOI Outb(0x20, 0x20)