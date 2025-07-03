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
#include <global.h>
#include <io/kio.h>
#include <types/nums.h>

/// Size of a disk sector in bytes
#define SECTOR_SIZE 512

/// FAT16 partition type code variant 1
#define FAT16_TYPE1 0x04

/// FAT16 partition type code variant 2
#define FAT16_TYPE2 0x06

/**
 * @struct PartitionEntry
 * @brief Represents a single partition entry in the MBR partition table.
 */
struct PKG PartitionEntry {
    u8 status;                /**< Bootable flag: 0x80 = bootable, 0x00 = non-bootable */
    u8 chs_first[3];          /**< CHS address of first absolute sector in partition */
    u8 type;                  /**< Partition type (e.g., FAT16 types 0x04, 0x06) */
    u8 chs_last[3];           /**< CHS address of last absolute sector in partition */
    u32 lba_first_sector;     /**< LBA of first absolute sector in partition */
    u32 total_sectors;        /**< Total sectors in partition */
};

/**
 * @struct MBR
 * @brief Master Boot Record structure layout.
 */
struct PKG MBR {
    u8 bootstrap[446];              /**< Bootstrap code area */
    struct PartitionEntry partitions[4]; /**< Partition table entries */
    u16 signature;                  /**< Signature bytes, must be 0xAA55 */
};

/// Buffer for reading sectors
u8 sector[SECTOR_SIZE];

/**
 * @brief Finds the LBA start of the first FAT16 partition in the MBR.
 *
 * Reads the MBR sector, verifies the signature, then scans all partitions
 * for a FAT16 type (0x04 or 0x06).  
 *
 * Prints debug info about partitions to kernel log.
 *
 * @return LBA of the FAT16 partition start or 0 if none found or invalid MBR.
 */
u32 FindFat16Partition() {
    AtaReadSector(0, sector);
    Kprintf("Read ATA Sector\n");

    struct MBR *mbr = (struct MBR *)sector;

    if (mbr->signature != 0xAA55) {
        return 0; ///< Invalid MBR signature
    }

    Kprintf("Valid MBR Signature 0xAA55 Detected\n");

    for (int i = 0; i < 4; i++) {
        struct PartitionEntry *part = &mbr->partitions[i];
        Kprintf("Partition type: 0x%x %x\n", i, part->type);
        Kprintf(", LBA start: %x\n", part->lba_first_sector);

        Kprintf("Partition dump: %x", i);
        for (int b = 0; b < (int)sizeof(struct PartitionEntry); b++) {
            KprintHex(((u8 *)part)[b]);
            Kputchar(' ');
        }
        Kputchar('\n');

        if (part->type == FAT16_TYPE1 || part->type == FAT16_TYPE2) {
            return part->lba_first_sector; ///< Return FAT16 partition start LBA
        }
    }

    return 0; ///< No FAT16 partition found
}