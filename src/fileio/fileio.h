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

#pragma once
#include <global.h>
#include <types/nums.h>

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

#define ATA_WRITE_CMD 0x30

struct PKG FaT16Bpb {
    u8 jmp[3];
    u8 oem[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sectors;
    u8 num_fats;
    u16 root_entry_count;
    u16 total_sectors_short;
    u8 media_descriptor;
    u16 sectors_per_fat;
    u16 sectors_per_track;
    u16 num_heads;
    u32 hidden_sectors;
    u32 total_sectors_long;

    u8 drive_number;
    u8 reserved1;
    u8 boot_signature;
    u32 volume_id;
    u8 volume_label[11];
    u8 fs_type[8];
};

struct FaT16Info {
    u32 fat_start_lba;
    u32 root_dir_start_lba;
    u32 data_start_lba;

    u16 root_entry_count;
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u8 num_fats;
    u16 sectors_per_fat;

    u16 reserved_sectors;
    u16 total_sectors_16;
    u32 total_sectors_32;
    u32 total_cluster;
};

extern struct FaT16Info fat16;

void AtaReadSector(u32 lba, u8 *buffer);
void AtaWriteSector(u32 lba, const u8 *buffer);

extern volatile u8 irq14stat;
extern volatile u8 irq15stat;

u32 FindFat16Partition();

void Writefile(const char *filename, const char *ext, const char *data,
               u32 size);
void Fat16Ls();
u32 Fat16Mount(u32 partition_lba);
void Fileconts(const char *filename, const char *ext);
void Mkfile(const char *filename, const char *ext);
void Fat16RemoveFile(const char *filename, const char *ext);

static inline void Padname(const char *src, char *dest, int len) {
    int i = 0;
    while (i < len && src[i] != '\0') {
        char ch = src[i];
        dest[i] = (ch >= 'a' && ch <= 'z') ? (ch - 32) : ch; // Uppercase
        i++;
    }
    while (i < len) {
        dest[i++] = ' '; // Space-pad
    }
}