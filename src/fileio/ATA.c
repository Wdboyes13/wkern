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

#include <err/kerror.h>
#include <fileio/fileio.h>
#include <io/kio.h>
#include <utils/util.h>

void AtaPoll() {
    for (int i = 0; i < 4; i++) {
        Inb(ATA_CTRL); // 400ns delay
    }

    while (Inb(ATA_STATUS) & ATA_SR_BSY) {
        ; // Wait for BSY to clear
    }
    u8 status = Inb(ATA_STATUS);
    if (status & ATA_SR_ERR) {
        Panic("ATA ERR in poll");
    }
    if (!(status & ATA_SR_DRQ)) {
        Panic("ATA DRQ not set in poll");
    }
}

void AtaReadSector(u32 lba, u8 *buffer) {
    irq14stat = 0;
    Outb(0x3F6, 0x00); // clear SRST, enable IRQs
    Outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    for (int i = 0; i < 4; i++) {
        Inb(0x3F6);
    }
    Outb(ATA_SECCNT, 1);
    Outb(ATA_LBA_LOW, lba & 0xFF);
    Outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    Outb(ATA_LBA_HI, (lba >> 16) & 0xFF);
    Outb(ATA_COMMAND, ATA_CMD_READ_SECTORS);

    AtaPoll();

    u8 status = Inb(ATA_STATUS);
    u8 err = Inb(ATA_ERROR);

    for (int i = 0; i < 256; i++) {
        u16 data = Inw(ATA_DATA);
        buffer[i * 2] = (u8)(data & 0xFF);
        buffer[(i * 2) + 1] = (u8)(data >> 8);
    }
}

void AtaWriteSector(u32 lba, const u8 *buffer) {
    Outb(ATA_CTRL, 0x00); // enable IRQs

    Outb(ATA_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F)); // drive + LBA mode
    Outb(ATA_IO_BASE + 2, 1);                           // sector count
    Outb(ATA_IO_BASE + 3, (u8)(lba));                   // LBA low
    Outb(ATA_IO_BASE + 4, (u8)(lba >> 8));              // LBA mid
    Outb(ATA_IO_BASE + 5, (u8)(lba >> 16));             // LBA high

    Outb(ATA_IO_BASE + 7, ATA_WRITE_CMD); // send write command

    // wait for drive to be ready
    while (!(Inb(ATA_IO_BASE + 7) & 0x08)) {
        ; // DRQ
    }

    // write data
    for (int i = 0; i < 256; i++) {
        u16 word = ((u16)buffer[(i * 2) + 1] << 8) | buffer[i * 2];
        Outw(ATA_IO_BASE, word);
    }

    // Flush cache
    Outb(ATA_IO_BASE + 7, 0xE7); // flush command
    while (Inb(ATA_IO_BASE + 7) & 0x80) {
        ; // wait for BSY to clear
    }
}