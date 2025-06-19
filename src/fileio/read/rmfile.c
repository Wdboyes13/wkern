#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>

void fat16_remove_file(const char *filename, const char *ext) {
    kuint32_t entries_per_sector = fat16.bytes_per_sector / 32;
    kuint32_t root_dir_sectors =
        ((fat16.root_entry_count * 32) + fat16.bytes_per_sector - 1) /
        fat16.bytes_per_sector;
    kuint8_t sector[512];

    for (kuint32_t i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, sector);
        for (kuint32_t j = 0; j < entries_per_sector; j++) {
            kuint8_t *entry = &sector[j * 32];

            if (entry[0] == 0x00)
                return; // no more entries

            if (entry[0] == 0xE5)
                continue; // already deleted

            // Check filename and extension match (make sure you pad and compare
            // exactly 8 and 3 bytes)
            if (kmemcmp(entry, filename, 8) == 0 &&
                kmemcmp(entry + 8, ext, 3) == 0) {
                // Mark entry as deleted
                entry[0] = 0xE5;

                // Write back directory sector
                ata_write_sector(fat16.root_dir_start_lba + i, sector);

                // Free FAT clusters
                kuint16_t cluster = entry[26] | (entry[27] << 8);
                while (cluster >= 0x0002 && cluster <= 0xFFEF) {
                    kuint32_t fat_offset = cluster * 2;
                    kuint32_t fat_sector =
                        fat16.fat_start_lba + (fat_offset / 512);
                    kuint32_t fat_index = fat_offset % 512;

                    kuint8_t fatbuf[512];
                    ata_read_sector(fat_sector, fatbuf);

                    // Read next cluster in chain
                    kuint16_t next_cluster =
                        fatbuf[fat_index] | (fatbuf[fat_index + 1] << 8);

                    // Clear current cluster in FAT
                    fatbuf[fat_index] = 0x00;
                    fatbuf[fat_index + 1] = 0x00;
                    ata_write_sector(fat_sector, fatbuf);

                    cluster = next_cluster;
                }
                return; // Done deleting file
            }
        }
    }
}