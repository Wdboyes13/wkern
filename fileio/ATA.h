#pragma once

#include <types/nums.h>

static void ata_wait();
void ata_read_sector(kuint32_t lba, kuint8_t* buffer);
void ata_write_sector(kuint32_t lba, const kuint8_t *buffer);