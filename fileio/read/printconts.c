#include <fileio/ATA.h>
#include <fileio/fat16.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <types/nums.h>
#include <utils/katoi.h>

void fileconts() {
    char cluster_ascii[20];
    kprintf("Enter Cluster Code: ");
    kgetstr(cluster_ascii, 19);
    int cluster = katoi_auto(cluster_ascii);
    kputchar('\n');
    kuint32_t lba =
        fat16.data_start_lba + (cluster - 2) * fat16.sectors_per_cluster;
    kprintf("Reading LBA: ");
    kprint_hex(lba);
    kputchar('\n');
    kprintf(" -- Contents --\n");
    kuint8_t fsector[512];
    ata_read_sector(lba, fsector);
    for (int i = 0; i < 512; i++) {
        if (fsector[i] >= 32 && fsector[i] <= 126) {
            kputchar(fsector[i]);
        } else {
            kputchar('.');
        }
    }
    kputchar('\n');
}