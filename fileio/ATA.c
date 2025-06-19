#include <err/panic.h>
#include <fileio/irqflags.h>
#include <io/asm.h>
#include <io/printer.h>
#include <utils/ksleep.h>

#define ATA_IO_BASE 0x1F0
#define ATA_CTRL 0x3F6
#define ATA_STATUS (ATA_IO_BASE + 7)
#define ATA_COMMAND (ATA_IO_BASE + 7)
#define ATA_DRIVE (ATA_IO_BASE + 6)

#define ATA_ERROR (ATA_IO_BASE + 1)
#define ATA_SECCNT (ATA_IO_BASE + 2)
#define ATA_LBA_LOW (ATA_IO_BASE + 3)
#define ATA_LBA_MID (ATA_IO_BASE + 4)
#define ATA_LBA_HI (ATA_IO_BASE + 5)

#define ATA_DATA (ATA_IO_BASE + 0)

#define ATA_CMD_READ_SECTORS 0x20

#define ATA_SR_BSY 0x80
#define ATA_SR_DRQ 0x08
#define ATA_SR_ERR 0x01

void ata_poll() {
    for (int i = 0; i < 4; i++)
        inb(ATA_CTRL); // 400ns delay

    while (inb(ATA_STATUS) & ATA_SR_BSY)
        ; // Wait for BSY to clear
    kuint8_t status = inb(ATA_STATUS);
    if (status & ATA_SR_ERR)
        panic("ATA ERR in poll");
    if (!(status & ATA_SR_DRQ))
        panic("ATA DRQ not set in poll");
}

void ata_read_sector(kuint32_t lba, kuint8_t *buffer) {
    irq14stat = 0;
    kprintf("Reading sector ");
    kprint_hex(lba);
    kputchar('\n');
    outb(0x3F6, 0x00); // clear SRST, enable IRQs
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    for (int i = 0; i < 4; i++)
        inb(0x3F6);
    outb(ATA_SECCNT, 1);
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HI, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND, ATA_CMD_READ_SECTORS);

    ata_poll();

    kuint8_t status = inb(ATA_STATUS);
    kuint8_t err = inb(ATA_ERROR);
    kprintf("Status after command: ");
    kprint_hex(status);
    kputchar('\n');
    kprintf("Error: ");
    kprint_hex(err);
    kputchar('\n');

    for (int i = 0; i < 256; i++) {
        kuint16_t data = inw(ATA_DATA);
        buffer[i * 2] = (kuint8_t)(data & 0xFF);
        buffer[i * 2 + 1] = (kuint8_t)(data >> 8);
    }
}