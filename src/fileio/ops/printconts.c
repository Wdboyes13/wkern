#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <types/nums.h>
#include <utils/util.h>

void fileconts(const char *filename, const char *ext) {
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
                u32 lba = fat16.data_start_lba +
                                ((clust - 2) * fat16.sectors_per_cluster);

                u8 data[512] = {0};
                ata_read_sector(lba, data);
                for (int i = 0; i < 512; i++) {
                    if (data[i] == '\r' && data[i + 1] == '\n') {
                        break; // CRLF = end of message
                    }

                    if (data[i] == 0x20) {
                        kputchar(' ');
                    } else if (data[i] <= 0x40 || data[i] >= 0x7B) {
                        kputchar('.');
                    } else {
                        kputchar(data[i]);
                    }
                }
                kputchar('\n');
            }
        }
    }
}