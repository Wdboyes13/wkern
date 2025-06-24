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
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define VIRTIO_REG(offset) (iob + offset)

#include <global.h>
#include <io/kio.h>

u8 virtio_irq = 0;
u8 virtio_bus = 0;
u8 virtio_slot = 0;
u8 virtio_func = 0;

u32 bar0 = 0;

u32 pci_config_read(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 addr =
        (1U << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, addr);
    return inl(PCI_CONFIG_DATA);
}

u32 find_virtionet_dev() {
    for (u8 bus = 0; bus < 255; bus++) {
        for (u8 slot = 0; slot < 32; slot++) {
            for (u8 func = 0; func < 8; func++) {
                u16 vendor = pci_config_read(bus, slot, func, 0x00) & 0xFFFF;
                if (vendor == 0xFFFF)
                    continue;
                u16 dev =
                    (pci_config_read(bus, slot, func, 0x00) >> 16) & 0xFFFF;
                bar0 = pci_config_read(bus, slot, func, 0x10);
                u32 iob = bar0 & ~0xF;
                u32 revid =
                    (pci_config_read(bus, slot, func, 0x08) >> 0) & 0xFF;

                if (vendor == 0x1AF4 && dev == 0x1000 && revid == 0x00) {
                    virtio_bus = bus;
                    virtio_slot = slot;
                    virtio_func = func;
                    kprintf("VirtIO at %x:%x.%x BAR0=%x IO=%x Rev=%x\n", bus,
                            slot, func, bar0, iob, revid);

                    // 1. Reset device
                    outb(VIRTIO_REG(0x12), 0x00);

                    // 2. Set ACKNOWLEDGE and DRIVER status bits
                    outb(VIRTIO_REG(0x12), 0x01); // Acknowledge
                    outb(VIRTIO_REG(0x12), 0x03); // Acknowledge | Driver

                    // 3. Read device features
                    u32 features = inl(VIRTIO_REG(0x00));
                    // Optionally mask features here

                    // 4. Write accepted features
                    outl(VIRTIO_REG(0x04), features); // For now, accept all

                    // 5. Set DRIVER_OK bit
                    outb(VIRTIO_REG(0x12),
                         0x07); // Acknowledge | Driver | Features OK
                    return iob;
                }
            }
        }
    }
    return (u32)(uptr)NULL;
}