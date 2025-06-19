#include <KShell/shell.h>
#include <err/panic.h>
#include <fileio/ATA.h>
#include <fileio/MBR.h>
#include <fileio/fat16.h>
#include <global.h>
#include <idt/idtirq.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <types/nums.h>
#include <utils/ksleep.h>
char name[20];

void kernel_main() {

    all_idt();

    // kuint32_t lba = find_fat16_partition();
    // kprint_hex(lba);
    // if (!lba) {
    //    panic("No FAT16 partition found");
    // }

    if (!fat16_mount(0)) {
        panic("Failed to mount FAT16 volume");
    }
    kcfp();

    kprintf("\nHello form WKern!\n");
    kprintf("Enter your name: ");
    kgetstr(name, 19);
    kputchar('\n');
    kprintf("Hello, ");
    kprintf(name);
    kprintf("!\n");

    ZZZ(50);
    kflush();
    kprintf("Welcome!\n\nStarting SH\n");
    sh();
#ifdef VMQEMU
#include <qemu/shutdown.h>
    qemu_shutdown();
#endif
}
