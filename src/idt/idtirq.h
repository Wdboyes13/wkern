#pragma once

#include <global.h>
#include <types/nums.h> // put includes at the top

/*
===================
-- IDT FUNCTIONS --
===================
*/
void setup_idt(void);
void idt_init(void);
void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags);
void mask_all_irqs(void);
void unmask_irq(u8 irq);
void debug_print_idt_entry(int i);

void all_idt();

// PIC remap
extern void _picr(void);

/*
==================
---- STRUCTS -----
==================
*/

// IDT Entry structure (packed to prevent compiler padding)
struct idt_entry {
    u16 base_lo;
    u16 sel; // Kernel code segment selector
    u8 always0;
    u8 flags; // Flags (present, ring level, type)
    u16 base_hi;
} PKG;

struct gdt_entry {
    u16 limit_low;  // Lower 16 bits of limit
    u16 base_low;   // Lower 16 bits of base
    u8 base_middle; // Next 8 bits of base
    u8 access;      // Access flags
    u8 granularity; // Granularity flags + upper 4 bits of limit
    u8 base_high;   // Last 8 bits of base
} PKG;

struct gdt_ptr {
    u16 limit;
    u32 base;
} PKG;

struct PKG idt_ptr {
    u16 limit;
    u32 base;
};

/*
===================
-- GDT FUNCTIONS --
===================
*/
void gdt_install();
extern void gdt_flush(u32);
void gdt_set_gate(int num, u32 base, u32 limit, u8 access, u8 gran);

/*
============
-- OTHERS --
============
*/
extern struct idt_ptr idt_ptrn;
#define IDT_ENTRIES 256
extern struct idt_entry idt[IDT_ENTRIES];
void ms_sleep(unsigned int ms);

// ----- HANDLERS ----- //

void irq0_handler_c(void);      // C handler called from asm IRQ0 stub
extern void irq0_handler(void); // Assembly IRQ0 stub (global in asm)
extern void good_handler(void);
extern void irq1_handler(void);
