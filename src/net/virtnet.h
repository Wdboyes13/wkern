/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <types/nums.h>
#include <global.h>

#define VIRTIO_PCI_QUEUE_SEL 0x0E
#define VIRTIO_PCI_QUEUE_NUM 0x0C
#define VIRTIO_PCI_QUEUE_PFN 0x08
#define VIRTIO_PCI_QUEUE_NOTIFY 0x10
#define VIRTQ_DESC_F_NEXT 1
#define VIRTQ_DESC_F_WRITE 2
#define VIRTIO_MAX_SAFE_QSIZE 16
#define MAX_RX_DESCS VIRTIO_MAX_SAFE_QSIZE
#define VIRTIO_PCI_HOST_FEATURES   0x00
#define VIRTIO_PCI_GUEST_FEATURES  0x04
#define VIRTIO_PCI_STATUS          0x12

#define VIRTIO_STATUS_ACKNOWLEDGE 1
#define VIRTIO_STATUS_DRIVER      2
#define VIRTIO_STATUS_DRIVER_OK   4

typedef struct VirtqDesc {
    uptr addr;
    u32 len;
    u16 flags;
    u16 next;
} PKG virtq_desc;
typedef struct VirtqAvail {
    u16 flags;
    u16 idx;
    u16 ring[];
} PKG virtq_avail;
typedef struct VirtqUsed {
    u16 idx;
    u16 flags;
    struct {
        u32 id;
        u32 len;
    } ring[];
} PKG virtq_used;

void NetStq(u32 iob);
void VirtnetInit(u32 iob);
void VirtnetNegotiate(u32 iob);
void VirtnetSetup();