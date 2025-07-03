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

/** Interrupt Descriptor Table entries */
struct IdtEntry idt[IDT_ENTRIES];

/** IDT pointer structure, aligned to 16 bytes */
struct IdtPtr idt_ptrn __attribute__((aligned(16)));

/**
 * @brief Set an IDT gate (entry) at given index
 * 
 * @param num Index of the IDT entry to set
 * @param base Address of the interrupt handler function
 * @param sel Code segment selector in GDT
 * @param flags Flags controlling gate type and privileges
 */
void IdtSetGate(u8 num, u32 base, u16 sel, u8 flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

/** External assembly function to load the IDT */
extern void IdtLoad(void);

/**
 * @brief Debug function to print an IDT entry's handler address
 * 
 * @param i Index of the IDT entry to print
 */
void debug_print_idt_entry(int i) {
    u32 base = (idt[i].base_hi << 16) | idt[i].base_lo;
    char buf[25];
    Kitoa(i, buf);
    Kprintf("IDT[%s] = %x\n", buf, base);
}

/**
 * @brief Initialize and load the Interrupt Descriptor Table (IDT)
 * 
 * - Installs GDT
 * - Remaps PIC interrupts
 * - Masks all IRQs, then unmasks IRQ 0 and 1
 * - Sets IDT gates for IRQ0 and IRQ1 handlers
 * - Loads the IDT and enables interrupts
 * - Starts the PIT timer at a base frequency
 */
void AllIdt() {

    GdtInstall(); /**< Install the Global Descriptor Table (GDT) */

    Kprintf("Calling PIC Remap\n");
    _picr(); /**< Remap the PIC (Programmable Interrupt Controller) */

    MaskAllIrqs(); /**< Mask all IRQs for safety */

    UnmaskIrq(1); /**< Unmask IRQ1 (keyboard) */
    UnmaskIrq(0); /**< Unmask IRQ0 (timer) */

    Kprintf("Setting IDT to 0x00\n");
    Kmemset(&idt, 0,
            sizeof(struct IdtEntry) * IDT_ENTRIES); /**< Clear the IDT entries */

    Kprintf("Setting IDT Limit + Base\n");
    idt_ptrn.limit =
        (sizeof(struct IdtEntry) * IDT_ENTRIES) - 1; /**< Set IDT limit */
    idt_ptrn.base = (uptr)idt;                       /**< Set IDT base address */

    Kprintf("Setting up IDT gate 32 (IRQ0)\n");
    IdtSetGate(32, (uptr)Irq0Handler, KERNEL_CODE_SEGMENT,
               0x8E); /**< Set IRQ0 handler */
    debug_print_idt_entry(32);

    Kprintf("Setting up IDT gate 33 (IRQ1)\n");
    IdtSetGate(33, (uptr)Irq1Handler, KERNEL_CODE_SEGMENT,
               0x8E); /**< Set IRQ1 handler */
    debug_print_idt_entry(33);

    Kprintf("Disabling interrupts\n");
    __asm__ volatile("cli"); /**< Disable CPU interrupts during setup */

    struct {
        u16 limit;
        u32 base;
    } PKG gdtr;

    __asm__ volatile("sgdt %0" : "=m"(gdtr)); /**< Reload the GDT */

    Kprintf("GDT base = %x, limit = %x\n", gdtr.base, gdtr.limit);
    Kprintf("IDT base = %x, limit = %x\n", idt_ptrn.base, idt_ptrn.limit);

    Kprintf("Loading IDT\n");
    __asm__ volatile("lidt %[idt]" ::[idt] "m"(idt_ptrn)
                     : "memory"); /**< Load the new IDT */

    Kprintf("Enabling interrupts");
    __asm__ volatile("sti"); /**< Re-enable CPU interrupts */

    PitInit(PIT_FREQ); /**< Initialize PIT with base frequency (e.g., 1193182 Hz) */
}