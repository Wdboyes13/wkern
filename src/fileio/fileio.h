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

/** @file fileio.h
 *  @brief Definitions and declarations for FAT16 filesystem and ATA I/O.
 */

/// ATA IO Base port for primary IDE channel
#define ATA_IO_BASE 0x1F0

/// ATA Control port
#define ATA_CTRL 0x3F6

/// ATA Status port (read)
#define ATA_STATUS (ATA_IO_BASE + 7)

/// ATA Command port (write)
#define ATA_COMMAND (ATA_IO_BASE + 7)

/// ATA Drive/head select port
#define ATA_DRIVE (ATA_IO_BASE + 6)

/// ATA Error register port (read)
#define ATA_ERROR (ATA_IO_BASE + 1)

/// ATA Sector count register port
#define ATA_SECCNT (ATA_IO_BASE + 2)

/// ATA LBA low byte port
#define ATA_LBA_LOW (ATA_IO_BASE + 3)

/// ATA LBA mid byte port
#define ATA_LBA_MID (ATA_IO_BASE + 4)

/// ATA LBA high byte port
#define ATA_LBA_HI (ATA_IO_BASE + 5)

/// ATA Data port (read/write 16-bit)
#define ATA_DATA (ATA_IO_BASE + 0)

/// ATA command to read sectors
#define ATA_CMD_READ_SECTORS 0x20

/// ATA status register BSY (busy) bit
#define ATA_SR_BSY 0x80

/// ATA status register DRQ (data request) bit
#define ATA_SR_DRQ 0x08

/// ATA status register ERR (error) bit
#define ATA_SR_ERR 0x01

/// ATA command to write sectors
#define ATA_WRITE_CMD 0x30

/**
 * @struct FaT16Bpb
 * @brief FAT16 BIOS Parameter Block (BPB) structure — describes volume layout.
 *
 * Matches the FAT16 boot sector layout.
 */
struct PKG FaT16Bpb {
    u8 jmp[3];                 /**< Jump instruction to boot code */
    u8 oem[8];                 /**< OEM Name */
    u16 bytes_per_sector;      /**< Bytes per sector */
    u8 sectors_per_cluster;    /**< Sectors per cluster */
    u16 reserved_sectors;      /**< Reserved sectors before FAT */
    u8 num_fats;               /**< Number of FAT tables */
    u16 root_entry_count;      /**< Max root directory entries */
    u16 total_sectors_short;   /**< Total sectors (if zero, use total_sectors_long) */
    u8 media_descriptor;       /**< Media descriptor byte */
    u16 sectors_per_fat;       /**< Sectors per FAT */
    u16 sectors_per_track;     /**< Sectors per track (for BIOS) */
    u16 num_heads;             /**< Number of heads (for BIOS) */
    u32 hidden_sectors;        /**< Hidden sectors before partition */
    u32 total_sectors_long;    /**< Total sectors (if total_sectors_short is zero) */

    // Extended BPB fields
    u8 drive_number;           /**< Physical drive number */
    u8 reserved1;              /**< Reserved */
    u8 boot_signature;         /**< Extended boot signature (0x29 means valid) */
    u32 volume_id;             /**< Volume serial number */
    u8 volume_label[11];       /**< Volume label string */
    u8 fs_type[8];             /**< Filesystem type string (e.g., "FAT16   ") */
};

/**
 * @struct FaT16Info
 * @brief Parsed FAT16 filesystem layout and parameters.
 *
 * Populated after mounting the FAT16 partition.
 */
struct FaT16Info {
    u32 fat_start_lba;         /**< LBA start of FAT tables */
    u32 root_dir_start_lba;    /**< LBA start of root directory */
    u32 data_start_lba;        /**< LBA start of data region */

    u16 root_entry_count;      /**< Max root entries */
    u16 bytes_per_sector;      /**< Bytes per sector */
    u8 sectors_per_cluster;    /**< Sectors per cluster */
    u8 num_fats;               /**< Number of FAT tables */
    u16 sectors_per_fat;       /**< Sectors per FAT */

    u16 reserved_sectors;      /**< Reserved sectors count */
    u16 total_sectors_16;      /**< Total sectors (16-bit) */
    u32 total_sectors_32;      /**< Total sectors (32-bit) */
    u32 total_cluster;         /**< Total data clusters */
};

/// Global FAT16 filesystem info struct
extern struct FaT16Info fat16;

/**
 * @brief Reads a 512-byte sector from ATA disk into buffer.
 * @param lba Logical block address of sector.
 * @param buffer Pointer to buffer to store sector data (must be >= 512 bytes).
 */
void AtaReadSector(u32 lba, u8 *buffer);

/**
 * @brief Writes a 512-byte sector to ATA disk from buffer.
 * @param lba Logical block address of sector.
 * @param buffer Pointer to buffer containing data to write (must be >= 512 bytes).
 */
void AtaWriteSector(u32 lba, const u8 *buffer);

/// IRQ status flags for IRQ14 and IRQ15 (primary and secondary IDE interrupts)
extern volatile u8 irq14stat;
extern volatile u8 irq15stat;

/**
 * @brief Finds the FAT16 partition on disk.
 * @return LBA of FAT16 partition start.
 */
u32 FindFat16Partition(void);

/**
 * @brief Writes data to a FAT16 file, replacing its contents.
 * @param filename 8.3 filename (name only).
 * @param ext File extension (3 chars).
 * @param data Pointer to data buffer to write.
 * @param size Size of data to write in bytes.
 */
void Writefile(const char *filename, const char *ext, const char *data, u32 size);

/**
 * @brief Lists files in the FAT16 root directory.
 */
void Fat16Ls(void);

/**
 * @brief Mounts FAT16 partition and reads filesystem info.
 * @param partition_lba LBA address of FAT16 partition start.
 * @return 1 on success, 0 on failure.
 */
u32 Fat16Mount(u32 partition_lba);

/**
 * @brief Prints contents of a FAT16 file.
 * @param filename Name of the file (8 chars).
 * @param ext File extension (3 chars).
 */
void Fileconts(const char *filename, const char *ext);

/**
 * @brief Creates a new empty file in the FAT16 root directory.
 * @param filename File name (8 chars).
 * @param ext File extension (3 chars).
 */
void Mkfile(const char *filename, const char *ext);

/**
 * @brief Removes a file from the FAT16 root directory and frees clusters.
 * @param filename File name (8 chars).
 * @param ext File extension (3 chars).
 */
void Fat16RemoveFile(const char *filename, const char *ext);

/**
 * @brief Pads and uppercases a filename or extension for FAT16 8.3 format.
 * @param src Input string (filename or extension).
 * @param dest Output buffer to hold padded string.
 * @param len Length to pad/uppercase to (8 for filename, 3 for extension).
 *
 * Converts lowercase letters to uppercase, pads with spaces to required length.
 */
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