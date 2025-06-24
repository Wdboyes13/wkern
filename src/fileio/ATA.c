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

#include <err/kerror.h>
#include <fileio/fileio.h>
#include <io/kio.h>
#include <utils/util.h>

void ata_poll() {
    for (int i = 0; i < 4; i++)
        inb(ATA_CTRL); // 400ns delay

    while (inb(ATA_STATUS) & ATA_SR_BSY)
        ; // Wait for BSY to clear
    u8 status = inb(ATA_STATUS);
    if (status & ATA_SR_ERR)
        panic("ATA ERR in poll");
    if (!(status & ATA_SR_DRQ))
        panic("ATA DRQ not set in poll");
}

void ata_read_sector(u32 lba, u8 *buffer) {
    irq14stat = 0;
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

    u8 status = inb(ATA_STATUS);
    u8 err = inb(ATA_ERROR);

    for (int i = 0; i < 256; i++) {
        u16 data = inw(ATA_DATA);
        buffer[i * 2] = (u8)(data & 0xFF);
        buffer[i * 2 + 1] = (u8)(data >> 8);
    }
}

void ata_write_sector(u32 lba, const u8 *buffer) {
    outb(ATA_CTRL, 0x00); // enable IRQs

    outb(ATA_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F)); // drive + LBA mode
    outb(ATA_IO_BASE + 2, 1);                           // sector count
    outb(ATA_IO_BASE + 3, (u8)(lba));                   // LBA low
    outb(ATA_IO_BASE + 4, (u8)(lba >> 8));              // LBA mid
    outb(ATA_IO_BASE + 5, (u8)(lba >> 16));             // LBA high

    outb(ATA_IO_BASE + 7, ATA_WRITE_CMD); // send write command

    // wait for drive to be ready
    while (!(inb(ATA_IO_BASE + 7) & 0x08))
        ; // DRQ

    // write data
    for (int i = 0; i < 256; i++) {
        u16 word = ((u16)buffer[i * 2 + 1] << 8) | buffer[i * 2];
        outw(ATA_IO_BASE, word);
    }

    // Flush cache
    outb(ATA_IO_BASE + 7, 0xE7); // flush command
    while (inb(ATA_IO_BASE + 7) & 0x80)
        ; // wait for BSY to clear
}