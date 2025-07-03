/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
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
#define VIRTIO_REG(offset) (iob + (offset))

#include <global.h>
#include <io/kio.h>

u8 virtio_irq = 0;
u8 virtio_bus = 0;
u8 virtio_slot = 0;
u8 virtio_func = 0;

u32 bar0 = 0;

/**
 * @brief Reads a 32-bit value from PCI configuration space.
 *
 * This function constructs a configuration address based on the given
 * PCI bus, slot (device), function, and offset, and reads the corresponding
 * 32-bit value from the PCI configuration space using I/O ports.
 *
 * The offset must be aligned to a 4-byte boundary (i.e., bits 1 and 0 are
 * masked out).
 *
 * @param bus    PCI bus number (0–255).
 * @param slot   PCI device number (0–31).
 * @param func   PCI function number (0–7).
 * @param offset Offset into the configuration space (must be 4-byte aligned).
 * @return       32-bit value read from the PCI configuration register.
 */
u32 PciConfigRead(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 addr =
        (1U << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    Outl(PCI_CONFIG_ADDRESS, addr);
    return Inl(PCI_CONFIG_DATA);
}

/**
 * @brief Search for VirtNet Device
 * @return NULL
 */
u32 FindVirtionetDev() {
    for (u8 bus = 0; bus < 255; bus++) {
        for (u8 slot = 0; slot < 32; slot++) {
            for (u8 func = 0; func < 8; func++) {
                u16 vendor = PciConfigRead(bus, slot, func, 0x00) & 0xFFFF;
                if (vendor == 0xFFFF) {
                    continue;
                }
                u16 dev = (PciConfigRead(bus, slot, func, 0x00) >> 16) & 0xFFFF;
                bar0 = PciConfigRead(bus, slot, func, 0x10);
                u32 iob = bar0 & ~0xF;
                u32 revid = (PciConfigRead(bus, slot, func, 0x08) >> 0) & 0xFF;

                if (vendor == 0x1AF4 && dev == 0x1000 && revid == 0x00) {
                    virtio_bus = bus;
                    virtio_slot = slot;
                    virtio_func = func;
                    Kprintf("VirtIO at %x:%x.%x BAR0=%x IO=%x Rev=%x\n", bus,
                            slot, func, bar0, iob, revid);

                    // 1. Reset device
                    Outb(VIRTIO_REG(0x12), 0x00);

                    // 2. Set ACKNOWLEDGE and DRIVER status bits
                    Outb(VIRTIO_REG(0x12), 0x01); // AcKnowledge
                    Outb(VIRTIO_REG(0x12), 0x03); // AcKnowledge | Driver

                    // 3. Read device features
                    u32 features = Inl(VIRTIO_REG(0x00));
                    // Optionally masK features here

                    // 4. Write accepted features
                    Outl(VIRTIO_REG(0x04), features); // For now, accept all

                    // 5. Set DRIVER_OK bit
                    Outb(VIRTIO_REG(0x12),
                         0x07); // AcKnowledge | Driver | Features OK
                    return iob;
                }
            }
        }
    }
    return (u32)(uptr)NULL;
}