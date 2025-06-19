#include <idt/idtirq.h>
#include <io/kio.h>
#include <types/nums.h>
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void mask_all_irqs(void) {
    kprintf("Masking all IRQs\n");
    outb(PIC1_DATA, 0xFF); // Mask all IRQs on master PIC
    outb(PIC2_DATA, 0xFF); // Mask all IRQs on slave PIC
}

void unmask_irq(kuint8_t irq) {
    kprintf("Unmasking IRQ\n");
    kuint16_t port;
    kuint8_t value;

    if (irq < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}