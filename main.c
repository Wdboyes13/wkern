#include <err/panic.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <types/nums.h>
#include <utils/img.h>
#include <utils/katoi.h>
#include <utils/ksleep.h>
#include <wex/stwex.h>
#include <wex/testexec.h>
void kernel_main() {
    kcfp();
    kprintf("Hello form WKern!\n");

    kprintf("Enter your name: ");
    char name[20];
    kgetstr(name, 19);
    kputchar('\n');
    kprintf("Hello, ");
    kprintf(name);
    kprintf("!\n");

    ZZZ(50);
    kflush();
    kprintf("Enter your age: ");
    char age[5];
    kgetstr(age, 5);
    kputchar('\n');
    int iage = katoi(age);
    if (iage == 0 && age[0] != '0') {
        panic("Invalid age input!");
    }
    if (iage >= 13) {
        kprintf("Welcome buddy, your old enough!\n\n");
        draw_smile();
        runwex(execr());
    } else {
        panic("Underage user");
    }
    ZZZ(1000);
#ifdef VMQEMU
#include <qemu/shutdown.h>
    qemu_shutdown();
#endif
}
