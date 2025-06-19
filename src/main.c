#include <KShell/shell.h>
#include <err/kerror.h>
#include <fileio/fileio.h>
#include <idt/idtirq.h>
#include <io/kio.h>
#include <utils/util.h>

char name[20];

void kernel_main() {

    all_idt();

    kuint32_t lba = find_fat16_partition();
    kprint_hex(lba);
    if (!lba) {
        lba = 0;
    }

    if (!fat16_mount(lba)) {
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
}
