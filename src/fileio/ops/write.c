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

#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>

void writefile(const char *filename, const char *ext, const char *data,
               u32 size) {
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, sector);
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];

            if (entry[0] != 0x00 && entry[0] != 0xE5 &&
                kmemcmp(entry + 0x00, filename, 8) == 0 &&
                kmemcmp(entry + 0x08, ext, 3) == 0) {
                u16 clust = entry[0x1A] | (entry[0x1B] << 8);
                if (clust < 2) {
                    kprintf("Invalid Cluster\n");
                    return;
                }

                u32 data_sector = fat16.data_start_lba +
                                  ((clust - 2) * fat16.sectors_per_cluster);

                u8 writebuf[512] = {0};
                kmemcpy(writebuf, data, size < 512 ? size : 512);
                ata_write_sector(data_sector, writebuf);

                entry[0x1C] = size & 0xFF;
                entry[0x1D] = (size >> 8) & 0xFF;
                entry[0x1E] = (size >> 16) & 0xFF;
                entry[0x1F] = (size >> 24) & 0xFF;

                ata_write_sector(fat16.root_dir_start_lba + i, sector);

                kprintf("Wrote to file");
                return;
            }
        }
    }
}