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
#include <utils/util.h>


/**
 * @brief Display the contents of a file in the FAT16 filesystem.
 *
 * This function searches the FAT16 root directory for a file matching the
 * specified filename and extension. If found, it reads the first data cluster
 * of the file and prints its contents as ASCII characters to the console.
 * Non-printable characters are displayed as dots ('.'). The reading stops
 * either at the end of the cluster or when a FAT16 EOF marker (0x1A) is found.
 *
 * @param filename The 8-character filename (without extension).
 * @param ext The 3-character file extension.
 *
 * @note The function assumes the FAT16 filesystem structure is accessible
 *       through the global `fat16` variable.
 */
void Fileconts(const char *filename, const char *ext) {
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        AtaReadSector(fat16.root_dir_start_lba + i, sector);
        for (u32 j = 0; j < entries_per_sector; j++) {
            // Padding entries to be exactly 8 and 3, and upercased (blame msft
            // for FAT16)
            u8 *entry = &sector[j * 32];
            char name_pad[8];
            char ext_pad[3];
            Padname(filename, name_pad, 8);
            Padname(ext, ext_pad, 3);

            if (entry[0] != 0x00 &&
                entry[0] != 0xE5 && // ChecK if entry is not deleted & exists
                Kmemcmp(entry + 0x00, name_pad, 8) == 0 && // Compare name
                Kmemcmp(entry + 0x08, ext_pad, 3) == 0) {  // Compare ext
                u16 clust = entry[0x1A] | (entry[0x1B] << 8);
                if (clust < 2) {
                    Kprintf("Invalid Cluster\n");
                    return;
                }
                u32 lba = fat16.data_start_lba +
                          ((clust - 2) * fat16.sectors_per_cluster);

                u8 data[512] = {0};
                AtaReadSector(lba, data); // Read data in the FAT16 Sector
                for (int i = 0; i < 512; i++) {
                    if (data[i] == 0x1A) {
                        break; // EOF
                    }
                    if (data[i] >= 0x20 && data[i] <= 0x7E) {
                        Kputchar(data[i]); // printable ASCII
                    } else {
                        Kputchar('.'); // non-printable gets a dot
                    }
                }
                Kputchar('\n');
            }
        }
    }
}