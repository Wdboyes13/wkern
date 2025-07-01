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

#include <fileio/fileio.h>
#include <io/kio.h>

struct FaT16Info fat16;
static u8 fat16_sector[512];

u32 Fat16TotalClusters(struct FaT16Info *fat) {
    // Use 32-bit total sectors if 16-bit is zero
    u32 total_sectors = fat->total_sectors_16 != 0 ? fat->total_sectors_16
                                                   : fat->total_sectors_32;

    // Root directory size in sectors
    u32 root_dir_sectors =
        ((fat->root_entry_count * 32) + (fat->bytes_per_sector - 1)) /
        fat->bytes_per_sector;

    // Data sectors = total - reserved - (number of FATs × size of each FAT) -
    // root dir
    u32 data_sectors = total_sectors - fat->reserved_sectors -
                       (fat->num_fats * fat->sectors_per_fat) -
                       root_dir_sectors;

    // Cluster count = how many clusters fit in data region
    return data_sectors / fat->sectors_per_cluster;
}

u32 fat16_mount(u32 partition_lba) {
    AtaReadSector(partition_lba, fat16_sector);

    struct FaT16Bpb *bpb = (struct FaT16Bpb *)fat16_sector;

    if (*(u16 *)&fat16_sector[510] != 0xAA55) {
        Kprintf("Invalid FAT16 boot sector signature\n");
        return 0;
    }

    fat16.bytes_per_sector = bpb->bytes_per_sector;
    fat16.sectors_per_cluster = bpb->sectors_per_cluster;
    fat16.reserved_sectors = bpb->reserved_sectors;
    fat16.num_fats = bpb->num_fats;
    fat16.root_entry_count = bpb->root_entry_count;
    fat16.sectors_per_fat = bpb->sectors_per_fat;

    fat16.total_sectors_16 = bpb->total_sectors_short;
    fat16.total_sectors_32 = bpb->total_sectors_long;

    fat16.fat_start_lba = partition_lba + bpb->reserved_sectors;

    u32 root_dir_sectors =
        ((bpb->root_entry_count * 32) + (bpb->bytes_per_sector - 1)) /
        bpb->bytes_per_sector;

    fat16.root_dir_start_lba =
        fat16.fat_start_lba + (bpb->num_fats * bpb->sectors_per_fat);
    fat16.data_start_lba = fat16.root_dir_start_lba + root_dir_sectors;

    fat16.total_cluster = Fat16TotalClusters(&fat16);

    Kprintf("FAT16 Mount Success:\n");
    Kprintf(" FAT LBA: %x\n", fat16.fat_start_lba);
    Kprintf(" Root Dir LBA: %x\n", fat16.root_dir_start_lba);
    Kprintf(" Data LBA: %x\n", fat16.data_start_lba);

    return 1;
}