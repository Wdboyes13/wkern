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

#include <global.h>
#include <idt/idtirq.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>
#include <utils/util.h>

struct IdtEntry idt[IDT_ENTRIES];
struct IdtPtr idt_ptrn __attribute__((aligned(16)));
// Function to set an IDT gate
void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

// External asm function to load IDT (defined below)
extern void IdtLoad(void);

// Initialize the IDT table (call this from your Kernel init)

void debug_print_idt_entry(int i) {
    u32 base = (idt[i].base_hi << 16) | idt[i].base_lo;
    char buf[25];
    Kitoa(i, buf);
    Kprintf("IDT[%s] = %x\n", buf, base);
}

void all_idt() {

    GdtInstall(); // Install the GDT (Grand Descriptor Table)

    Kprintf("Calling PIC Remap\n");
    _picr(); // Remap the PIC

    MaskAllIrqs(); // MasK ALL IRQs for safety

    UnmaskIrq(1); // Then unmasK IRQ 1&0 because we use them
    UnmaskIrq(0);

    Kprintf("Setting IDT to 0x00\n");
    Kmemset(&idt, 0,
            sizeof(struct IdtEntry) *
                IDT_ENTRIES); // Setting the current IDT (Interrupt Descriptor
                              // Table) to 0

    Kprintf("Setting IDT Limit + Base\n");
    idt_ptrn.limit =
        (sizeof(struct IdtEntry) * IDT_ENTRIES) - 1; // Set the limit of the IDT
    idt_ptrn.base = (uptr)idt;                       // Set the base of the IDT

    Kprintf("Setting up IDT gate 32 (IRQ0)\n");
    idt_set_gate(32, (uptr)Irq0Handler, KERNEL_CODE_SEGMENT,
                 0x8E); // Load IRQ0 Handler into IDT Entry 32
    debug_print_idt_entry(32);

    Kprintf("Setting up IDT gate 33 (IRQ1)\n");
    idt_set_gate(33, (uptr)Irq1Handler, KERNEL_CODE_SEGMENT,
                 0x8E); // Load IRQ1 Handler into IDT Entry 33
    debug_print_idt_entry(33);

    // idt_set_gate(0, (uptr)virtnet_handler, KERNEL_CODE_SEGMENT, 0x8E);

    Kprintf("Disabling interrupts\n");
    __asm__ volatile("cli"); // Disable CPU Interrupts so the CPU doesnt reset

    struct {
        u16 limit;
        u32 base;
    } PKG gdtr;

    __asm__ volatile("sgdt %0" : "=m"(gdtr)); // Reload the GDT

    // DEBUG STUFF

    Kprintf("GDT base = %x, limit = %x\n", gdtr.base, gdtr.limit);
    Kprintf("IDT base = %x, limit = %x\n", idt_ptrn.base, idt_ptrn.limit);

    Kprintf("Loading IDT\n");
    __asm__ volatile("lidt %[idt]" ::[idt] "m"(idt_ptrn)
                     : "memory"); // Load the new IDT

    Kprintf("Enabling interrupts");
    __asm__ volatile("sti"); // Reenable CPU Interrupts

    PitInit(PIT_FREQ); // Start the PIT (Programmable Interval Timer) at
                       // Frequency of 119 (10ms/ticK)
}