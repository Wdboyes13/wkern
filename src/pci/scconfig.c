#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define VIRTIO_REG(offset) (iob + offset)

#include <io/kio.h>

u32 pci_config_read(u8 bus, u8 slot, u8 func, u8 offset) {
    u32 addr =
        (1U << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, addr);
    return inl(PCI_CONFIG_DATA);
}

void printdevinfo(u8 bus, u8 slot, u8 func, u16 dev, u16 vendor) {
    kprintf("Vendor: ");
    kprint_hex(vendor);
    kputchar('\n');
    kprintf("Found VirtIO Device at ");
    kprint_hex(bus);
    kputchar(':');
    kprint_hex(slot);
    kputchar('.');
    kprint_hex(func);
    kprintf(" Device ID: ");
    kprint_hex(dev);
    kputchar('\n');
}

void find_virtionet_dev() {
    for (u8 bus = 0; bus < 255; bus++) {
        for (u8 slot = 0; slot < 32; slot++) {
            for (u8 func = 0; func < 8; func++) {
                u16 vendor = pci_config_read(bus, slot, func, 0x00) & 0xFFFF;
                if (vendor == 0xFFFF)
                    continue;
                u16 dev =
                    (pci_config_read(bus, slot, func, 0x00) >> 16) & 0xFFFF;
                u32 bar0 = pci_config_read(bus, slot, func, 0x10);
                u32 iob = bar0 & ~0xF;
                u32 revid =
                    (pci_config_read(bus, slot, func, 0x08) >> 0) & 0xFF;

                if (vendor == 0x1AF4 && dev == 0x1000 && revid == 0x00) {
                    printdevinfo(bus, slot, func, dev, vendor);

                    kprintf("Bar 0: ");
                    kprint_hex(bar0);
                    kprintf(" I/O Base: ");
                    kprint_hex(iob);
                    kputchar('\n');
                    kprintf("RevID: ");
                    kprint_hex(revid);
                    kputchar('\n');

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
                }
            }
        }
    }
}