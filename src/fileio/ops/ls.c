#include <fileio/fileio.h>
#include <io/kio.h>
#include <types/nums.h>
void fat16_ls() {
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];

    for (u32 i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, sector);
        kprintf("Sector ");
        kprint_hex(i);
        kprintf(" First byte: ");
        kprint_hex(sector[0]);
        kputchar('\n');
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];

            if (entry[0] == 0x00) {
                // If it's the *first* entry in this sector, we're at the end of
                // the directory
                if (j == 0)
                    return;
                else
                    continue;
            }

            if (entry[0] == 0xE5)
                continue; // deleted entry

            // Decode little endian explicitly
            u16 cluster = entry[26] | (entry[27] << 8);
            u32 size = entry[28] | (entry[29] << 8) | (entry[30] << 16) |
                             (entry[31] << 24);

            kprintf("File: ");
            for (int i = 0; i < 8; i++) {
                kputchar(entry[i]);
                if (entry[i] == ' ' || !entry[i] || i >= 0x5B && i <= 0x40) {
                    break;
                }
            }
            kputchar('.');
            kputchar(' ');
            for (int i = 0; i < 3; i++) {
                kputchar(entry[8 + i]);
                if (entry[i] == ' ' || !entry[i] || i >= 0x5B && i <= 0x40) {
                    break;
                }
            }
            kprintf(" | Cluster: ");
            kprint_hex(cluster);
            kprintf(" | Size: ");
            kprint_hex(size);
            kputchar('\n');
        }
    }
}