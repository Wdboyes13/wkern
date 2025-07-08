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

typedef struct {
    u16 offset_low;  // bits 0-15 of handler addr
    u16 selector;    // GDT code segment selector
    u8 ist;          // Interrupt Stack Table offset (0 usually)
    u8 type_attr;    // flags: P, DPL, type (0x8E for interrupt gate)
    u16 offset_mid;  // bits 16-31 of handler addr
    u32 offset_high; // bits 32-63 of handler addr
    u32 zero;        // reserved
} PKG IDTEntry64;

struct GdtEntry {
    u16 limit_low;  // Lower 16 bits of limit
    u16 base_low;   // Lower 16 bits of base
    u8 base_middle; // Next 8 bits of base
    u8 access;      // Access flags
    u8 granularity; // Granularity flags + upper 4 bits of limit
    u8 base_high;   // Last 8 bits of base
} PKG;

struct GdtEntry64 {
    u16 limit_low;  // bits 0-15 of limit
    u16 base_low;   // bits 0-15 of base
    u8 base_middle; // bits 16-23 of base
    u8 access;      // access byte
    u8 granularity; // flags and limit bits 16-19
    u8 base_high;   // bits 24-31 of base
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
void GdtInstall64();
extern void GdtFlush(u32);
void GdtSetGate(int num, u32 base, u32 limit, u8 access, u8 gran);
void GdtSetGate64(int num, u32 base, u32 limit, u8 access, u8 gran);

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
#define GDT_ACCESS_CODE64                                                      \
    0x9A // 1 00 1 1 0 1 0: Present, Ring 0, Code segment, Executable, Readable
#define GDT_ACCESS_DATA64                                                      \
    0x92 // 1 00 1 0 0 1 0: Present, Ring 0, Data segment, Writable

#define GDT_GRANULARITY64                                                      \
    0xA0 // 1010 0000: Long mode bit (L-bit) set, 4K granularity off for code
#define GDT_GRANULARITY_DATA64                                                 \
    0xC0 // 1100 0000: Granularity set, default operation size (32-bit) for data