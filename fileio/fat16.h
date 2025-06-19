#pragma once

#include <types/nums.h>

kuint32_t fat16_mount(kuint32_t partition_lba);

struct __attribute__((packed)) FAT16_BPB {
    kuint8_t  jmp[3];
    kuint8_t  oem[8];
    kuint16_t bytes_per_sector;
    kuint8_t  sectors_per_cluster;
    kuint16_t reserved_sectors;
    kuint8_t  num_fats;
    kuint16_t root_entry_count;
    kuint16_t total_sectors_short;
    kuint8_t  media_descriptor;
    kuint16_t sectors_per_fat;
    kuint16_t sectors_per_track;
    kuint16_t num_heads;
    kuint32_t hidden_sectors;
    kuint32_t total_sectors_long;

    kuint8_t  drive_number;
    kuint8_t  reserved1;
    kuint8_t  boot_signature;
    kuint32_t volume_id;
    kuint8_t  volume_label[11];
    kuint8_t  fs_type[8];
};

struct FAT16_Info {
    kuint32_t fat_start_lba;
    kuint32_t root_dir_start_lba;
    kuint32_t data_start_lba;
    
    kuint16_t root_entry_count;
    kuint16_t bytes_per_sector;
    kuint8_t  sectors_per_cluster;
    kuint8_t  num_fats;
    kuint16_t sectors_per_fat;
    kuint16_t reserved_sectors; // ← Add this field
};