#include <err/kerror.h>
#include <io/kio.h>
#include <utils/util.h>
__attribute__((noreturn)) void qemu_shutdown() {
    kprintf("\nShutting Down...\n");
    ZZZ(200);
    outw(0x604, 0x2000);  // Magic value to ACPI shutdown in QEMU
    outw(0xB004, 0x2000); // Backup for older versions
    triple_fault();
    __builtin_unreachable();
}