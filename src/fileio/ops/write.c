#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>

void writefile(const char *filename, const char *ext, const char *data,
               kuint32_t size) {
    kuint32_t entries_per_sector = fat16.bytes_per_sector / 32;
    kuint32_t root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    kuint8_t sector[512];

    for (kuint32_t i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, sector);
        for (kuint32_t j = 0; j < entries_per_sector; j++) {
            kuint8_t *entry = &sector[j * 32];

            if (entry[0] != 0x00 && entry[0] != 0xE5 &&
                kmemcmp(entry + 0x00, filename, 8) == 0 &&
                kmemcmp(entry + 0x08, ext, 3) == 0) {
                kuint16_t clust = entry[0x1A] | (entry[0x1B] << 8);
                if (clust < 2) {
                    kprintf("Invalid Cluster\n");
                    return;
                }

                kuint32_t data_sector =
                    fat16.data_start_lba +
                    ((clust - 2) * fat16.sectors_per_cluster);

                kuint8_t writebuf[512] = {0};
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