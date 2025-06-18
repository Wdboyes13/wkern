#include "utils/ksleep.h"
#include <idt/idtirq.h>
#include <io/asm.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <mem/memset.h>
#include <types/nums.h>
#include <utils/katoi.h>

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptrn __attribute__((aligned(16)));
// Function to set an IDT gate
void idt_set_gate(kuint8_t num, kuint32_t base, kuint16_t sel, kuint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

// External asm function to load IDT (defined below)
extern void idt_load(void);

// Initialize the IDT table (call this from your kernel init)

#define KERNEL_CODE_SEGMENT 0x08 // Usually your GDT code segment

void isr3_handler(void) { kputchar('!'); }

extern void isr3_stub(void);

void debug_print_idt_entry(int i) {
    kuint32_t base = (idt[i].base_hi << 16) | idt[i].base_lo;
    kprintf("IDT[");
    char buf[25];
    kitoa(i, buf);
    kprintf(buf);
    kprintf("] = ");
    kprint_hex(base);
    kprintf("\n");
}

void all_idt() {

    gdt_install();

    kprintf("Calling PIC Remap\n");
    _picr();

    kprintf("Setting IDT to 0x00\n");
    kmemset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES); // 🧼 clear first!

    kprintf("Setting IDT Limit + Base\n");
    idt_ptrn.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_ptrn.base = (kuintptr_t)idt;

    kprintf("Setting up IDT gate 3\n");
    idt_set_gate(3, (kuintptr_t)isr3_stub, KERNEL_CODE_SEGMENT, 0x8E);
    debug_print_idt_entry(3);

    kprintf("Setting up IDT gate 32 (IRQ0)\n");
    idt_set_gate(32, (kuintptr_t)irq0_handler, KERNEL_CODE_SEGMENT, 0x8E);
    debug_print_idt_entry(32);

    kprintf("Setting up IDT gate 33 (IRQ1)\n");
    idt_set_gate(33, (kuintptr_t)irq1_handler, 0x08, 0x8E);
    debug_print_idt_entry(33);

    kprintf("Disabling interrupts\n");
    __asm__ volatile("cli");

    // DEBUG STUFF

    struct {
        kuint16_t limit;
        kuint32_t base;
    } __attribute__((packed)) gdtr;

    __asm__ volatile("sgdt %0" : "=m"(gdtr));

    kprintf("GDT base = ");
    kprint_hex(gdtr.base);
    kprintf(", limit = ");
    kprint_hex(gdtr.limit);
    kputchar('\n');

    kprintf("IDT base = ");
    kprint_hex(idt_ptrn.base);
    kprintf(", limit = ");
    kprint_hex(idt_ptrn.limit);
    kputchar('\n');

    // FINAL STUFF

    kprintf("Loading IDT\n");
    __asm__ volatile("lidt %[idt]" ::[idt] "m"(idt_ptrn) : "memory");

    kprintf("Enabling interrupts");
    __asm__ volatile("sti");

    kprintf("Calling test INT\n");
    __asm__ volatile("int $3");

    pit_init(100);
}