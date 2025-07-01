#include <idt/idtirq.h> // Your IDT and IRQ helper headers
#include <io/kio.h>
#include <mem/kmem.h>
#include <net/virtnet.h>
#include <pci/pci.h>
#include <types/nums.h>

static u32
    virtio_iob; // Store BAR0 base I/O address globally or pass appropriately

void VirtnetIrqHandler() {
    // Read ISR to acKnowledge interrupt from VirtIO device
    u8 isr = Inb(virtio_iob + 0x13);

    if (isr & 0x1) {
        // Call your pacKet handler to process used buffers
        NetStq(virtio_iob);
    }
}