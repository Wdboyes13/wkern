#include <fileio/ATA.h>
#include <fileio/fat16.h>
#include <io/printer.h>

struct FAT16_Info fat16;
static kuint8_t fat16_sector[512];

kuint32_t fat16_mount(kuint32_t partition_lba) {
    ata_read_sector(partition_lba, fat16_sector);

    struct FAT16_BPB *bpb = (struct FAT16_BPB *)fat16_sector;

    if (*(kuint16_t *)&fat16_sector[510] != 0xAA55) {
        kprintf("Invalid FAT16 boot sector signature\n");
        return 0;
    }

    fat16.bytes_per_sector = bpb->bytes_per_sector;
    fat16.sectors_per_cluster = bpb->sectors_per_cluster;
    fat16.reserved_sectors = bpb->reserved_sectors;
    fat16.num_fats = bpb->num_fats;
    fat16.root_entry_count = bpb->root_entry_count;
    fat16.sectors_per_fat = bpb->sectors_per_fat;

    fat16.fat_start_lba = partition_lba + bpb->reserved_sectors;

    kuint32_t root_dir_sectors =
        ((bpb->root_entry_count * 32) + (bpb->bytes_per_sector - 1)) /
        bpb->bytes_per_sector;

    fat16.root_dir_start_lba =
        fat16.fat_start_lba + (bpb->num_fats * bpb->sectors_per_fat);
    fat16.data_start_lba = fat16.root_dir_start_lba + root_dir_sectors;

    kprintf("FAT16 Mount Success:\n");
    kprintf(" FAT LBA: ");
    kprint_hex(fat16.fat_start_lba);
    kputchar('\n');
    kprintf(" Root Dir LBA: ");
    kprint_hex(fat16.root_dir_start_lba);
    kputchar('\n');
    kprintf(" Data LBA: ");
    kprint_hex(fat16.data_start_lba);
    kputchar('\n');

    return 1;
}