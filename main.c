#include <err/panic.h>
#include <err/tf.h>
#include <idt/idtirq.h>
#include <io/asm.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <types/nums.h>
#include <utils/img.h>
#include <utils/katoi.h>
#include <utils/ksleep.h>
#include <wex/stwex.h>
#include <wex/testexec.h>
void kernel_main() {
    // pic_remap();

    // idt_init();

    // setup_idt();

    // idt_load();

    // cli();

    // pit_init(100);

    // sti();
    // kprintf("After STI - interrupts enabled\n");

    kcfp();
    kprintf("kcfp done\n");

    kprintf("\nHello form WKern!\n");
    kprintf("Enter your name: ");
    char name[20];
    kgetstr(name, 19);
    kputchar('\n');
    kprintf("Hello, ");
    kprintf(name);
    kprintf("!\n");

    ZZZ(50);
    kflush();
    kprintf("Welcome!\n\n");
    draw_smile();
    ZZZ(1000);
    kprintf("CTIWT");
    runwex(execr());
#ifdef VMQEMU
#include <qemu/shutdown.h>
    qemu_shutdown();
#endif
}
