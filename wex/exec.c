#include <io/printer.h>
#include <utils/ksleep.h>
#include <wex/exec.h>
#ifdef VMQEMU
#include <qemu/shutdown.h>
#endif
void exec(unsigned char *exec, int *FVALS) {
    int i = FVALS[0];
    kprintf("Executing WEX\n");
    if (exec[i] == 0x0A) { // print
        kprintf("Printing from WEX\n");
        i++; // move to start of string
        while (exec[i] != 0x00) {
            kputchar(exec[i]);
            i++;
        }
        i++; // move past 0x00
    }

    if (exec[i] == 0x0C && exec[i + 1] == 0x00) { // shutdown
        kprintf("Shitting down from WEX\n");
#ifdef VMQEMU
        qemu_shutdown();
#endif
        if (exec[i] == 0x0D && exec[i + 1] == 0x00) {
            i++;
            ZZZ(1000);
            i++;
        }
    }
}