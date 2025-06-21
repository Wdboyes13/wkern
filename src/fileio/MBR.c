#include <fileio/fileio.h>
#include <global.h>
#include <io/kio.h>
#include <types/nums.h>

#define SECTOR_SIZE 512
#define FAT16_TYPE1 0x04
#define FAT16_TYPE2 0x06

struct PKG PartitionEntry {
    u8 status;
    u8 chs_first[3];
    u8 type;
    u8 chs_last[3];
    u32 lba_first_sector;
    u32 total_sectors;
};

struct PKG MBR {
    u8 bootstrap[446];
    struct PartitionEntry partitions[4];
    u16 signature;
};

u8 sector[SECTOR_SIZE];

u32 find_fat16_partition() {
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
            kprint_hex(((u8 *)part)[b]);
            kputchar(' ');
        }
        kputchar('\n');

        if (part->type == FAT16_TYPE1 || part->type == FAT16_TYPE2) {
            return part->lba_first_sector;
        }
    }
    return 0;
}
