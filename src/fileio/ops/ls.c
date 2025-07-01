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
#include <types/nums.h>
void Fat16Ls() {
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        AtaReadSector(fat16.root_dir_start_lba + i, sector);
        Kprintf("Sector %x First Byte: %x\n", i, sector[0]);
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];

            if (entry[0] == 0x00) {
                // If it's the *first* entry in this sector, we're at the end of
                // the directory
                if (j == 0) {
                    return;
                }
                continue;
            }

            if (entry[0] == 0xE5) {
                continue; // deleted entry
            }

            // Decode little endian explicitly
            u16 cluster = entry[26] | (entry[27] << 8);
            u32 size = entry[28] | (entry[29] << 8) | (entry[30] << 16) |
                       (entry[31] << 24);

            Kprintf("File: ");
            for (int i = 0; i < 8; i++) {
                Kputchar(entry[i]);
                if (entry[i] == ' ' || !entry[i] || i >= 0x5B && i <= 0x40) {
                    break;
                }
            }
            Kputchar('.');
            Kputchar(' ');
            for (int i = 0; i < 3; i++) {
                Kputchar(entry[8 + i]);
                if (entry[i] == ' ' || !entry[i] || i >= 0x5B && i <= 0x40) {
                    break;
                }
            }
            Kprintf(" | Cluster: %x | Size: %x\n", cluster, size);
        }
    }
}