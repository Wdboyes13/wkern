#include <idt/idtirq.h>
#include <io/asm.h> // your outb function or define it
void pic_remap() {
    outb(0x20, 0x11); // Start PIC1 init
    outb(0xA0, 0x11); // Start PIC2 init

    outb(0x21, 0x20); // PIC1 vector offset (IRQ0 = 32)
    outb(0xA1, 0x28); // PIC2 vector offset (IRQ8 = 40)

    outb(0x21, 0x04); // Tell PIC1 about PIC2 at IRQ2
    outb(0xA1, 0x02); // Tell PIC2 it’s on IRQ2

    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01);

    outb(0x21, 0x0); // Unmask all
    outb(0xA1, 0x0);
}