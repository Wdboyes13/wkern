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

/**
 * @brief Holds FAT16 filesystem info and a sector buffer.
 */
struct FaT16Info fat16;
static u8 fat16_sector[512];

/**
 * @brief Calculate the total number of data clusters in a FAT16 filesystem.
 *
 * @param fat Pointer to a FaT16Info struct representing the FAT16 filesystem.
 * @return Number of clusters available in the data region.
 *
 * This calculates clusters by subtracting reserved, FAT, and root directory sectors
 * from the total sectors and dividing by sectors per cluster.
 */
u32 Fat16TotalClusters(struct FaT16Info *fat) {
    // Use 32-bit total sectors if 16-bit is zero (fat16 magic)
    u32 total_sectors = fat->total_sectors_16 != 0 ? fat->total_sectors_16
                                                   : fat->total_sectors_32;

    // Root directory size in sectors (each entry is 32 bytes)
    u32 root_dir_sectors =
        ((fat->root_entry_count * 32) + (fat->bytes_per_sector - 1)) /
        fat->bytes_per_sector;

    // Calculate data sectors by subtracting reserved, FAT tables, and root dir
    u32 data_sectors = total_sectors - fat->reserved_sectors -
                       (fat->num_fats * fat->sectors_per_fat) -
                       root_dir_sectors;

    // Total clusters in data region
    return data_sectors / fat->sectors_per_cluster;
}

/**
 * @brief Mounts the FAT16 filesystem from a given partition LBA.
 *
 * Reads the boot sector, validates the FAT16 signature, and initializes the
 * FAT16 info struct with important filesystem layout details.
 *
 * @param partition_lba Logical block address of the FAT16 partition.
 * @return 1 on success, 0 on failure (invalid boot sector signature).
 *
 * Prints mount status and key filesystem layout info via Kprintf.
 */
u32 Fat16Mount(u32 partition_lba) {
    AtaReadSector(partition_lba, fat16_sector);

    struct FaT16Bpb *bpb = (struct FaT16Bpb *)fat16_sector;

    // Check boot sector signature 0xAA55
    if (*(u16 *)&fat16_sector[510] != 0xAA55) {
        Kprintf("Invalid FAT16 boot sector signature\n");
        return 0;
    }

    // Load BPB values into fat16 struct
    fat16.bytes_per_sector = bpb->bytes_per_sector;
    fat16.sectors_per_cluster = bpb->sectors_per_cluster;
    fat16.reserved_sectors = bpb->reserved_sectors;
    fat16.num_fats = bpb->num_fats;
    fat16.root_entry_count = bpb->root_entry_count;
    fat16.sectors_per_fat = bpb->sectors_per_fat;

    fat16.total_sectors_16 = bpb->total_sectors_short;
    fat16.total_sectors_32 = bpb->total_sectors_long;

    fat16.fat_start_lba = partition_lba + bpb->reserved_sectors;

    // Calculate root directory sectors and their start LBA
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