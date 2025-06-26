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

#include <idt/idtirq.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <net/virtnet.h>
#include <pci/pci.h>
#include <types/nums.h>
static u16 qsize = 0;
static virtq_desc *desc = NULL;
static virtq_avail *avail = NULL;
static virtq_used *used = NULL;
static void *virtq_rx = NULL;
static void *rx_bufs[MAX_RX_DESCS]; // define somewhere
static u16 last_used_idx = 0;

void virtnet_negotiate(u32 iob) {
    kprintf("Reset device status\n");
    outb(iob + VIRTIO_PCI_STATUS, 0);
    kprintf("Set ACKNOWLEDGE\n");
    outb(iob + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACKNOWLEDGE);
    kprintf("Set DRIVER\n");
    outb(iob + VIRTIO_PCI_STATUS,
         VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER);
    kprintf("Reading host features\n");
    u32 host_features = inl(iob + VIRTIO_PCI_HOST_FEATURES);
    kprintf("Host features: 0x");
    kprint_hex(host_features);
    kputchar('\n');
    kprintf("Writing guest features\n");
    outl(iob + VIRTIO_PCI_GUEST_FEATURES, 0);
    kprintf("Set DRIVER_OK\n");
    outb(iob + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACKNOWLEDGE |
                                      VIRTIO_STATUS_DRIVER |
                                      VIRTIO_STATUS_DRIVER_OK);
    kprintf("Negotiation done\n");
}

void virtnet_init(u32 iob) {
    outw(iob + VIRTIO_PCI_QUEUE_SEL, 0);
    kprintf("Allocating mem\n");
    virtq_rx = kmalloc(4096, 4096);
    if (!virtq_rx) {
        kprintf("Malloc Failed at virtnet_init");
        return;
    }

    u16 raw_qsize = inw(iob + VIRTIO_PCI_QUEUE_NUM);
    kprintf("Raw qsize: 0x%x\n", raw_qsize);

    // Clamp regardless of device size
    qsize = raw_qsize;
    if (qsize == 0 || qsize > VIRTIO_MAX_SAFE_QSIZE) {
        kprintf("Clamping qsize from %d to %d\n", qsize, VIRTIO_MAX_SAFE_QSIZE);
        qsize = VIRTIO_MAX_SAFE_QSIZE;
    }

    kmemset(virtq_rx, 0, 4096);

    kprintf("Setting descriptors, avail, and used\n");
    desc = (virtq_desc *)virtq_rx;
    avail = (virtq_avail *)(desc + qsize);
    used = (virtq_used *)((u8 *)virtq_rx + 4096 - (6 + 8 * qsize));
    int failed = 0;
    for (int i = 0; i < qsize; i++) {
        rx_bufs[i] = kmalloc(2048, 4096);
        if (!rx_bufs[i]) {
            kprintf("Failed to alloc rx_buf %d\n", i);
            failed++;
            continue;
        }
        desc[i].addr = (uptr)rx_bufs[i];
        desc[i].len = 2048;
        desc[i].flags = VIRTQ_DESC_F_WRITE;
        avail->ring[i] = i;
    }

    kprintf("Finishing net setup\n");
    kprintf("Failed RX_BUF: %d, Final qsize = %d\n", failed, qsize);

    avail->idx = qsize;
    kprintf("Sending signal 1\n");
    outl(iob + VIRTIO_PCI_QUEUE_PFN, ((uptr)virtq_rx) >> 12);
    kprintf("BAR0: %x\n", bar0);
    kprintf("Sending signal 2\n");
    outw(iob + VIRTIO_PCI_QUEUE_NOTIFY, 0);
    kprintf("Network up\n");
}

void net_stq(u32 iob) {
    // u8 isr = inb(iob + 0x13);
    // if (!(isr & 0x1))
    //     return; // no packets

    while (last_used_idx != used->idx) {
        u16 idx = last_used_idx % qsize;
        u32 desc_id = used->ring[idx].id;
        u32 len = used->ring[idx].len;

        char *packet = (char *)(uptr)desc[desc_id].addr;
        kprintf("Received packet (%d bytes):\n", len);
        for (u32 b = 0; b < len; b++) {
            kputchar(packet[b]);
        }

        // Re-add descriptor to avail ring so device can reuse it
        avail->ring[avail->idx % qsize] = desc_id;
        avail->idx++;

        last_used_idx++;
    }

    // Notify device new buffers are available
    outw(iob + VIRTIO_PCI_QUEUE_NOTIFY, 0);
}

void virtnet_setup() {
    iob = find_virtionet_dev();
    virtio_irq = pci_config_read(virtio_bus, virtio_slot, virtio_func,
                                 PCI_INTERRUPT_LINE) &
                 0xFF;
    extern void virtirq(void);
    idt_set_gate(virtio_irq + 32, (uptr)virtnet_irq_handler,
                 KERNEL_CODE_SEGMENT, 0x8E);
    unmask_irq(virtio_irq);

    virtnet_negotiate(iob);
    virtnet_init(iob);
}