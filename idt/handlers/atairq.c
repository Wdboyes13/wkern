#include <err/panic.h>
#include <fileio/irqflags.h>
#include <global.h>
#include <io/asm.h>
#include <io/printer.h>
#include <types/nums.h>

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI 0x20

#define ATA_STATUS 0x1F7

void irq14_handler_C() {
    kprintf("[ATA IRQ14 FIRED]\n");
    kuint8_t status = inb(ATA_STATUS);
    if (status & 0x01) {
        panic("Error in ATA IRQ14 Handler");
    }

    irq14stat = 1;

    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

#define ATA_STATUS_SECONDARY                                                   \
    0x170 // Secondary ATA channel base port + 7 offset (0x177), but for status
          // 0x177, so +7 from 0x170 base

void irq15_handler_C() {
    kprintf("[ATA IRQ15 FIRED]\n");
    // Read status from secondary ATA channel (usually 0x177)
    kuint8_t status = inb(ATA_STATUS_SECONDARY);
    if (status & 0x01) {
        panic("Error in ATA IRQ15 Handler");
    }

    irq15stat = 1;
    // Send EOI to slave and master PICs
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);

    // Mark IRQ15 handled - example flag
}
