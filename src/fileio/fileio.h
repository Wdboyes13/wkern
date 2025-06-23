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

struct PKG FAT16_BPB {
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

struct FAT16_Info {
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

extern struct FAT16_Info fat16;

static void ata_wait();
void ata_read_sector(u32 lba, u8 *buffer);
void ata_write_sector(u32 lba, const u8 *buffer);

extern volatile u8 irq14stat;
extern volatile u8 irq15stat;

u32 find_fat16_partition();

void writefile(const char *filename, const char *ext, const char *data,
               u32 size);
void fat16_ls();
u32 fat16_mount(u32 partition_lba);
void fileconts(const char *filename, const char *ext);
void mkfile(const char *filename, const char *ext);
void fat16_remove_file(const char *filename, const char *ext);

static inline void padname(const char *src, char *dest, int len) {
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