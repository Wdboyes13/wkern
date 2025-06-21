#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>
void mkfile(const char *filename, const char *ext) {
    int free = -1;
    int sector_to_write = -1;
    u32 entries_per_sector = fat16.bytes_per_sector / 32;
    u32 root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    u8 sector[512];
    u16 clust = 0;
    u32 siz = 0; // For new files size is zero

    // Find free root directory entry
    for (u32 i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, sector);
        for (u32 j = 0; j < entries_per_sector; j++) {
            u8 *entry = &sector[j * 32];
            if (entry[0] == 0x00 || entry[0] == 0xE5) {
                free = j;
                sector_to_write = i;
                goto found_free_entry; // break out of both loops ASAP
            }
        }
    }

found_free_entry:
    if (free == -1 || sector_to_write == -1) {
        kprintf("No free root directory entry found!\n");
        return;
    }

    // Find free cluster
    for (u16 c = 2; c < fat16.total_cluster; c++) {
        u32 fat_offset = c * 2;
        u32 fat_sector = fat16.fat_start_lba + (fat_offset / 512);
        u32 fat_index = fat_offset % 512;

        u8 fatbuf[512];
        ata_read_sector(fat_sector, fatbuf);

        if (fatbuf[fat_index] == 0x00 && fatbuf[fat_index + 1] == 0x00) {
            clust = c;
            fatbuf[fat_index] = 0xFF;
            fatbuf[fat_index + 1] = 0xFF;
            ata_write_sector(fat_sector, fatbuf);
            break;
        }
    }

    if (clust == 0) {
        kprintf("No free cluster found!\n");
        return;
    }

    // Now write the root directory entry
    ata_read_sector(fat16.root_dir_start_lba + sector_to_write,
                    sector); // Re-read sector for safety
    u8 *entry = &sector[free * 32];

    // Clear entry bytes (optional but good practice)
    for (int i = 0; i < 32; i++) {
        entry[i] = 0x00;
    }

    kmemcpy(entry + 0x00, filename, 8); // File Name
    kmemcpy(entry + 0x08, ext, 3);      // File Extension
    entry[0x0B] = 0x20;                 // Attribute: Archive

    // Time and Date (optional, set 0 here)
    entry[0x0E] = 0;
    entry[0x0F] = 0;
    entry[0x10] = 0;
    entry[0x11] = 0;

    // First cluster (little endian)
    entry[0x1A] = clust & 0xFF;
    entry[0x1B] = (clust >> 8) & 0xFF;

    // File size = 0 for new file
    entry[0x1C] = 0;
    entry[0x1D] = 0;
    entry[0x1E] = 0;
    entry[0x1F] = 0;

    ata_write_sector(fat16.root_dir_start_lba + sector_to_write, sector);
    kprintf("File entry written successfully.\n");
}