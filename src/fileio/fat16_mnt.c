#include <fileio/fileio.h>
#include <io/kio.h>

struct FAT16_Info fat16;
static u8 fat16_sector[512];

u32 fat16_total_clusters(struct FAT16_Info *fat) {
    // Use 32-bit total sectors if 16-bit is zero
    u32 total_sectors = fat->total_sectors_16 != 0
                                  ? fat->total_sectors_16
                                  : fat->total_sectors_32;

    // Root directory size in sectors
    u32 root_dir_sectors =
        ((fat->root_entry_count * 32) + (fat->bytes_per_sector - 1)) /
        fat->bytes_per_sector;

    // Data sectors = total - reserved - (number of FATs × size of each FAT) -
    // root dir
    u32 data_sectors = total_sectors - fat->reserved_sectors -
                             (fat->num_fats * fat->sectors_per_fat) -
                             root_dir_sectors;

    // Cluster count = how many clusters fit in data region
    return data_sectors / fat->sectors_per_cluster;
}

u32 fat16_mount(u32 partition_lba) {
    ata_read_sector(partition_lba, fat16_sector);

    struct FAT16_BPB *bpb = (struct FAT16_BPB *)fat16_sector;

    if (*(u16 *)&fat16_sector[510] != 0xAA55) {
        kprintf("Invalid FAT16 boot sector signature\n");
        return 0;
    }

    fat16.bytes_per_sector = bpb->bytes_per_sector;
    fat16.sectors_per_cluster = bpb->sectors_per_cluster;
    fat16.reserved_sectors = bpb->reserved_sectors;
    fat16.num_fats = bpb->num_fats;
    fat16.root_entry_count = bpb->root_entry_count;
    fat16.sectors_per_fat = bpb->sectors_per_fat;

    fat16.total_sectors_16 = bpb->total_sectors_short;
    fat16.total_sectors_32 = bpb->total_sectors_long;

    fat16.fat_start_lba = partition_lba + bpb->reserved_sectors;

    u32 root_dir_sectors =
        ((bpb->root_entry_count * 32) + (bpb->bytes_per_sector - 1)) /
        bpb->bytes_per_sector;

    fat16.root_dir_start_lba =
        fat16.fat_start_lba + (bpb->num_fats * bpb->sectors_per_fat);
    fat16.data_start_lba = fat16.root_dir_start_lba + root_dir_sectors;

    fat16.total_cluster = fat16_total_clusters(&fat16);

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