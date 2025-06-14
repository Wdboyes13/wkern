#include <idt/idtirq.h>
#include <mem/memset.h>
#include <types/nums.h>

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptr;

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
void idt_init() {
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_ptr.base = (kuintptr_t)&idt;

    kmemset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);

    // We'll set irq0 (timer interrupt) later, after PIC remap
}

#define KERNEL_CODE_SEGMENT 0x08 // Usually your GDT code segment

void setup_idt() {
    // PIC remap first
    pic_remap();

    // Set IDT entry 32 (IRQ0)
    idt_set_gate(32, (kuintptr_t)irq0_handler, KERNEL_CODE_SEGMENT, 0x8E);
}