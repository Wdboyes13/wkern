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

void fat16_remove_file(const char *filename, const char *ext) {
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + fat16.bytes_per_sector - 1) /
        fat16.bytes_per_sector;
    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        AtaReadSector(fat16.root_dir_start_lba + i, sector);
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];

            if (entry[0] == 0x00) {
                return; // no more entries
            }

            if (entry[0] == 0xE5) {
                continue; // already deleted
            }

            char name_pad[8];
            char ext_pad[3];
            Padname(filename, name_pad, 8);
            Padname(ext, ext_pad, 3);

            // ChecK filename and extension match
            if (Kmemcmp(entry, name_pad, 8) == 0 &&
                Kmemcmp(entry + 8, ext_pad, 3) == 0) {
                // MarK entry as deleted
                entry[0] = 0xE5;

                // Write bacK directory sector
                AtaWriteSector(fat16.root_dir_start_lba + i, sector);

                // Free FAT clusters
                u16 cluster = entry[26] | (entry[27] << 8);
                while (cluster >= 0x0002 && cluster <= 0xFFEF) {
                    u32 fat_offset = cluster * 2;
                    u32 fat_sector = fat16.fat_start_lba + (fat_offset / 512);
                    u32 fat_index = fat_offset % 512;

                    u8 fatbuf[512];
                    AtaReadSector(fat_sector, fatbuf);

                    // Read next cluster in chain
                    u16 next_cluster =
                        fatbuf[fat_index] | (fatbuf[fat_index + 1] << 8);

                    // Clear current cluster in FAT
                    fatbuf[fat_index] = 0x00;
                    fatbuf[fat_index + 1] = 0x00;
                    AtaWriteSector(fat_sector, fatbuf);

                    cluster = next_cluster;
                }
                return; // Done deleting file
            }
        }
    }
}