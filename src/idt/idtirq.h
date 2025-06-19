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
void idt_set_gate(kuint8_t num, kuint32_t base, kuint16_t sel, kuint8_t flags);
void mask_all_irqs(void);
void unmask_irq(kuint8_t irq);
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
    kuint16_t base_lo;
    kuint16_t sel; // Kernel code segment selector
    kuint8_t always0;
    kuint8_t flags; // Flags (present, ring level, type)
    kuint16_t base_hi;
} PKG;

struct gdt_entry {
    kuint16_t limit_low;  // Lower 16 bits of limit
    kuint16_t base_low;   // Lower 16 bits of base
    kuint8_t base_middle; // Next 8 bits of base
    kuint8_t access;      // Access flags
    kuint8_t granularity; // Granularity flags + upper 4 bits of limit
    kuint8_t base_high;   // Last 8 bits of base
} PKG;

struct gdt_ptr {
    kuint16_t limit;
    kuint32_t base;
} PKG;

struct PKG idt_ptr {
    kuint16_t limit;
    kuint32_t base;
};

/*
===================
-- GDT FUNCTIONS --
===================
*/
void gdt_install();
extern void gdt_flush(kuint32_t);
void gdt_set_gate(int num, kuint32_t base, kuint32_t limit, kuint8_t access,
                  kuint8_t gran);

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
