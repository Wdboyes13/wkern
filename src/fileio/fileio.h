#pragma once
#include <global.h>
#include <types/nums.h>

struct PKG FAT16_BPB {
    kuint8_t jmp[3];
    kuint8_t oem[8];
    kuint16_t bytes_per_sector;
    kuint8_t sectors_per_cluster;
    kuint16_t reserved_sectors;
    kuint8_t num_fats;
    kuint16_t root_entry_count;
    kuint16_t total_sectors_short;
    kuint8_t media_descriptor;
    kuint16_t sectors_per_fat;
    kuint16_t sectors_per_track;
    kuint16_t num_heads;
    kuint32_t hidden_sectors;
    kuint32_t total_sectors_long;

    kuint8_t drive_number;
    kuint8_t reserved1;
    kuint8_t boot_signature;
    kuint32_t volume_id;
    kuint8_t volume_label[11];
    kuint8_t fs_type[8];
};

struct FAT16_Info {
    kuint32_t fat_start_lba;
    kuint32_t root_dir_start_lba;
    kuint32_t data_start_lba;

    kuint16_t root_entry_count;
    kuint16_t bytes_per_sector;
    kuint8_t sectors_per_cluster;
    kuint8_t num_fats;
    kuint16_t sectors_per_fat;

    kuint16_t reserved_sectors;
    kuint16_t total_sectors_16;
    kuint32_t total_sectors_32;
    kuint32_t total_cluster;
};

extern struct FAT16_Info fat16;

static void ata_wait();
void ata_read_sector(kuint32_t lba, kuint8_t *buffer);
void ata_write_sector(kuint32_t lba, const kuint8_t *buffer);

extern volatile kuint8_t irq14stat;
extern volatile kuint8_t irq15stat;

kuint32_t find_fat16_partition();

void writefile(const char *filename, const char *ext, const char *data,
               kuint32_t size);
void fat16_ls();
kuint32_t fat16_mount(kuint32_t partition_lba);
void fileconts(const char *filename, const char *ext);
void mkfile(const char *filename, const char *ext);
void fat16_remove_file(const char *filename, const char *ext);