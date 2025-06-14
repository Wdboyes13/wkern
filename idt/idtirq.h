#pragma once

#include <types/nums.h> // put includes at the top

// IDT functions
void setup_idt(void);
void idt_init(void);
void idt_set_gate(kuint8_t num, kuint32_t base, kuint16_t sel, kuint8_t flags);

// PIC remap
void pic_remap(void);

// IRQ0 handler declarations
void irq0_handler_c(void);      // C handler called from asm IRQ0 stub
extern void irq0_handler(void); // Assembly IRQ0 stub (global in asm)

extern void idt_load(void);

// IDT Entry structure (packed to prevent compiler padding)
struct idt_entry {
    kuint16_t base_lo;
    kuint16_t sel; // Kernel code segment selector
    kuint8_t always0;
    kuint8_t flags; // Flags (present, ring level, type)
    kuint16_t base_hi;
} __attribute__((packed));

// IDT pointer structure for lidt instruction
struct idt_ptr {
    kuint16_t limit;
    kuint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256

extern struct idt_entry idt[IDT_ENTRIES];
extern struct idt_ptr idt_ptr;

void ms_sleep(unsigned int ms);