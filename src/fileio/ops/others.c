#include <fileio/fileio.h>
#include <mem/kmem.h>
#include <utils/util.h>

// Pads and compares 8.3 names
int fat16_match_entry(const kuint8_t *entry, const char *fname,
                      const char *ext) {
    kuint8_t padded_fname[8], padded_ext[3];
    kmemset(padded_fname, 0x20, 8);
    kmemset(padded_ext, 0x20, 3);

    kmemcpy(padded_fname, fname, kmstrlen(fname));
    kmemcpy(padded_ext, ext, kmstrlen(ext));

    return (kmemcmp(entry, padded_fname, 8) == 0 &&
            kmemcmp(entry + 8, padded_ext, 3) == 0);
}

kuint8_t *fat16_find_entry(const char *filename, const char *ext,
                           kuint8_t *out_sector) {
    kuint32_t entries_per_sector = fat16.bytes_per_sector / 32;
    kuint32_t root_dir_sectors =
        ((fat16.root_entry_count * 32) + (fat16.bytes_per_sector - 1)) /
        fat16.bytes_per_sector;

    for (kuint32_t i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat16.root_dir_start_lba + i, out_sector);
        for (kuint32_t j = 0; j < entries_per_sector; j++) {
            kuint8_t *entry = &out_sector[j * 32];

            if (entry[0] == 0x00 || entry[0] == 0xE5)
                continue;

            if (fat16_match_entry(entry, filename, ext)) {
                return entry;
            }
        }
    }

    return NULL;
}