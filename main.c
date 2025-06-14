#include <err/panic.h>
#include <img.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <types/nums.h>
#include <utils/katoi.h>
#include <utils/ksleep.h>
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

    ZZZ(100);
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
    } else {
        panic("Underage user");
    }
}