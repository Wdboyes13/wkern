#include <io/printer.h>
__attribute__((noreturn)) void panic(const char *msg) {
    kprintf("\n\n** KERNEL PANIC **\n");
    kprintf("Message: ");
    kprintf(msg);
    kputchar('\n');
    kprintf("System Halted");

    for (;;)
        __asm__ volatile("cli\n"
                         "hlt");
}