#include <fileio/ATA.h>
#include <global.h>
#include <io/printer.h>
#include <types/nums.h>

#define SECTOR_SIZE 512
#define FAT16_TYPE1 0x04
#define FAT16_TYPE2 0x06

struct PKG PartitionEntry {
    kuint8_t status;
    kuint8_t chs_first[3];
    kuint8_t type;
    kuint8_t chs_last[3];
    kuint32_t lba_first_sector;
    kuint32_t total_sectors;
};

struct PKG MBR {
    kuint8_t bootstrap[446];
    struct PartitionEntry partitions[4];
    kuint16_t signature;
};

kuint8_t sector[SECTOR_SIZE];

kuint32_t find_fat16_partition() {
    ata_read_sector(0, sector);
    kprintf("Read ATA Sector\n");
    struct MBR *mbr = (struct MBR *)sector;

    if (mbr->signature != 0xAA55) {
        return 0;
    } else {
        kprintf("Valid MBR Signature 0xAA55 Detected\n");
    }
    for (int i = 0; i < 4; i++) {
        struct PartitionEntry *part = &mbr->partitions[i];
        kprintf("Partition type: 0x");
        kprint_hex(i);
        kprint_hex(part->type);
        kprintf(", LBA start: ");
        kprint_hex(part->lba_first_sector);
        kputchar('\n');
        kprintf("Partition dump: ");
        kprint_hex(i);
        for (int b = 0; b < sizeof(struct PartitionEntry); b++) {
            kprint_hex(((kuint8_t *)part)[b]);
            kputchar(' ');
        }
        kputchar('\n');

        if (part->type == FAT16_TYPE1 || part->type == FAT16_TYPE2) {
            return part->lba_first_sector;
        }
    }
    return 0;
}
