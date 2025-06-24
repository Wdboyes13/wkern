#include <idt/idtirq.h> // Your IDT and IRQ helper headers
#include <io/kio.h>
#include <mem/kmem.h>
#include <net/virtnet.h>
#include <pci/pci.h>
#include <types/nums.h>

extern void send_eoi(u8 irq);

static u32
    virtio_iob; // Store BAR0 base I/O address globally or pass appropriately

void virtnet_irq_handler() {
    // Read ISR to acknowledge interrupt from VirtIO device
    u8 isr = inb(virtio_iob + 0x13);

    if (isr & 0x1) {
        // Call your packet handler to process used buffers
        net_stq(virtio_iob);
    }
}