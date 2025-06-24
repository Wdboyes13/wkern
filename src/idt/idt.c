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

#include <global.h>
#include <idt/idtirq.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>
#include <utils/util.h>

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptrn __attribute__((aligned(16)));
// Function to set an IDT gate
void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

// External asm function to load IDT (defined below)
extern void idt_load(void);

// Initialize the IDT table (call this from your kernel init)

void debug_print_idt_entry(int i) {
    u32 base = (idt[i].base_hi << 16) | idt[i].base_lo;
    char buf[25];
    kitoa(i, buf);
    kprintf("IDT[%s] = %x\n", buf, base);
}

void all_idt() {

    gdt_install(); // Install the GDT (Grand Descriptor Table)

    kprintf("Calling PIC Remap\n");
    _picr(); // Remap the PIC

    mask_all_irqs(); // Mask ALL IRQs for safety

    unmask_irq(1); // Then unmask IRQ 1&0 because we use them
    unmask_irq(0);

    kprintf("Setting IDT to 0x00\n");
    kmemset(&idt, 0,
            sizeof(struct idt_entry) *
                IDT_ENTRIES); // Setting the current IDT (Interrupt Descriptor
                              // Table) to 0

    kprintf("Setting IDT Limit + Base\n");
    idt_ptrn.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) -
                     1;        // Set the limit of the IDT
    idt_ptrn.base = (uptr)idt; // Set the base of the IDT

    kprintf("Setting up IDT gate 32 (IRQ0)\n");
    idt_set_gate(32, (uptr)irq0_handler, KERNEL_CODE_SEGMENT,
                 0x8E); // Load IRQ0 Handler into IDT Entry 32
    debug_print_idt_entry(32);

    kprintf("Setting up IDT gate 33 (IRQ1)\n");
    idt_set_gate(33, (uptr)irq1_handler, KERNEL_CODE_SEGMENT,
                 0x8E); // Load IRQ1 Handler into IDT Entry 33
    debug_print_idt_entry(33);

    kprintf("Disabling interrupts\n");
    __asm__ volatile("cli"); // Disable CPU Interrupts so the CPU doesnt reset

    struct {
        u16 limit;
        u32 base;
    } PKG gdtr;

    __asm__ volatile("sgdt %0" : "=m"(gdtr)); // Reload the GDT

    // DEBUG STUFF

    kprintf("GDT base = %x, limit = %x\n", gdtr.base, gdtr.limit);
    kprintf("IDT base = %x, limit = %x\n", idt_ptrn.base, idt_ptrn.limit);

    kprintf("Loading IDT\n");
    __asm__ volatile("lidt %[idt]" ::[idt] "m"(idt_ptrn)
                     : "memory"); // Load the new IDT

    kprintf("Enabling interrupts");
    __asm__ volatile("sti"); // Reenable CPU Interrupts

    pit_init(119); // Start the PIT (Programmable Interval Timer) at Frequency
                   // of 119 (10ms/tick)
}