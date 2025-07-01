/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOut ANY WARRANTY; withOut even the implied warranty of
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

void VirtnetNegotiate(u32 iob) {
    Kprintf("Reset device status\n");
    Outb(iob + VIRTIO_PCI_STATUS, 0);
    Kprintf("Set ACKNOWLEDGE\n");
    Outb(iob + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACKNOWLEDGE);
    Kprintf("Set DRIVER\n");
    Outb(iob + VIRTIO_PCI_STATUS,
         VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER);
    Kprintf("Reading host features\n");
    u32 host_features = Inl(iob + VIRTIO_PCI_HOST_FEATURES);
    Kprintf("Host features: 0x");
    KprintHex(host_features);
    Kputchar('\n');
    Kprintf("Writing guest features\n");
    Outl(iob + VIRTIO_PCI_GUEST_FEATURES, 0);
    Kprintf("Set DRIVER_OK\n");
    Outb(iob + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACKNOWLEDGE |
                                      VIRTIO_STATUS_DRIVER |
                                      VIRTIO_STATUS_DRIVER_OK);
    Kprintf("Negotiation done\n");
}

void VirtnetInit(u32 iob) {
    Outw(iob + VIRTIO_PCI_QUEUE_SEL, 0);
    Kprintf("Allocating mem\n");
    virtq_rx = Kmalloc(4096, 4096);
    if (!virtq_rx) {
        Kprintf("Malloc Failed at virtnet_init");
        return;
    }

    u16 raw_qsize = Inw(iob + VIRTIO_PCI_QUEUE_NUM);
    Kprintf("Raw qsize: 0x%x\n", raw_qsize);

    // Clamp regardless of device size
    qsize = raw_qsize;
    if (qsize == 0 || qsize > VIRTIO_MAX_SAFE_QSIZE) {
        Kprintf("Clamping qsize from %d to %d\n", qsize, VIRTIO_MAX_SAFE_QSIZE);
        qsize = VIRTIO_MAX_SAFE_QSIZE;
    }

    Kmemset(virtq_rx, 0, 4096);

    Kprintf("Setting descriptors, avail, and used\n");
    desc = (virtq_desc *)virtq_rx;
    avail = (virtq_avail *)(desc + qsize);
    used = (virtq_used *)((u8 *)virtq_rx + 4096 - (6 + 8 * qsize));
    int failed = 0;
    for (int i = 0; i < qsize; i++) {
        rx_bufs[i] = Kmalloc(2048, 4096);
        if (!rx_bufs[i]) {
            Kprintf("Failed to alloc rx_buf %d\n", i);
            failed++;
            continue;
        }
        desc[i].addr = (uptr)rx_bufs[i];
        desc[i].len = 2048;
        desc[i].flags = VIRTQ_DESC_F_WRITE;
        avail->ring[i] = i;
    }

    Kprintf("Finishing net setup\n");
    Kprintf("Failed RX_BUF: %d, Final qsize = %d\n", failed, qsize);

    avail->idx = qsize;
    Kprintf("Sending signal 1\n");
    Outl(iob + VIRTIO_PCI_QUEUE_PFN, ((uptr)virtq_rx) >> 12);
    Kprintf("BAR0: %x\n", bar0);
    Kprintf("Sending signal 2\n");
    Outw(iob + VIRTIO_PCI_QUEUE_NOTIFY, 0);
    Kprintf("NetworK up\n");
}

void NetStq(u32 iob) {
    // u8 isr = inb(iob + 0x13);
    // if (!(isr & 0x1))
    //     return; // no pacKets
    static const char hexchars[] = "0123456789ABCDEF";
    while (last_used_idx != used->idx) {
        u16 idx = last_used_idx % qsize;
        u32 desc_id = used->ring[idx].id;
        u32 len = used->ring[idx].len;

        char *pacKet = (char *)desc[desc_id].addr;
        Kprintf("Received pacKet (%d bytes):\n", len);
        for (u32 b = 0; b < len; b++) {
            u8 byte = pacKet[b];
            Kputchar(hexchars[(byte >> 4) & 0xF]);
            Kputchar(hexchars[byte & 0xF]);
            Kputchar(' ');
        }
        Kputchar('\n');

        // Re-add descriptor to avail ring so device can reuse it
        avail->ring[avail->idx % qsize] = desc_id;
        avail->idx++;

        last_used_idx++;
    }

    // Notify device new buffers are available
    Outw(iob + VIRTIO_PCI_QUEUE_NOTIFY, 0);
}

void VirtnetSetup() {
    iob = FindVirtionetDev();
    virtio_irq = PciConfigRead(virtio_bus, virtio_slot, virtio_func,
                               PCI_INTERRUPT_LINE) &
                 0xFF;
    extern void Virtirq(void);
    IdtSetGate(virtio_irq + 32, (uptr)VirtnetIrqHandler, KERNEL_CODE_SEGMENT,
               0x8E);
    UnmaskIrq(virtio_irq);

    VirtnetNegotiate(iob);
    VirtnetInit(iob);
}