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
#include <mem/kmem.h>
#include <types/nums.h>

/**
 * @brief Write data to an existing file in the FAT16 filesystem.
 *
 * Searches for the file specified by filename and extension in the FAT16 root
 * directory. If found, writes up to 512 bytes of data to the file's first
 * cluster sector and updates the file size in the directory entry.
 *
 * @param filename The 8-character filename (without extension).
 * @param ext The 3-character file extension.
 * @param data Pointer to the data buffer to write.
 * @param size Number of bytes to write from the data buffer (max 512 bytes).
 *
 * @note This function only writes to the first cluster of the file.
 *       Writing beyond the first cluster or appending is not handled.
 * @note Assumes the global `fat16` structure is properly initialized and
 * accessible.
 */
void Writefile(const char *filename, const char *ext, const char *data,
               u32 size) {
    Kprintf("Writing");
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        AtaReadSector(fat16.root_dir_start_lba + i, sector);
        Kprintf("ChecKing for file\n");
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];

            char name_pad[8];
            char ext_pad[3];
            Padname(filename, name_pad, 8);
            Padname(ext, ext_pad, 3);

            if (entry[0] != 0x00 && entry[0] != 0xE5 &&
                Kmemcmp(entry + 0x00, name_pad, 8) == 0 &&
                Kmemcmp(entry + 0x08, ext_pad, 3) == 0) {
                u16 clust = entry[0x1A] | (entry[0x1B] << 8);
                Kprintf("Found File\n");
                if (clust < 2) {
                    Kprintf("Invalid Cluster\n");
                    return;
                }

                u32 data_sector = fat16.data_start_lba +
                                  ((clust - 2) * fat16.sectors_per_cluster);

                u8 writebuf[512] = {0};
                Kmemcpy(writebuf, data,
                        size < 512 ? size
                                   : 512); // memcpy data into buffer to write
                AtaWriteSector(data_sector, writebuf); // write data to sector

                Kprintf("Size: %x\n", size);
                entry[0x1C] = size & 0xFF; // Update entries
                entry[0x1D] = (size >> 8) & 0xFF;
                entry[0x1E] = (size >> 16) & 0xFF;
                entry[0x1F] = (size >> 24) & 0xFF;

                AtaWriteSector(fat16.root_dir_start_lba + i,
                               sector); // Write updated entries

                Kprintf("Wrote to file");
                return;
            }
        }
    }
}