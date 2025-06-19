#include <fileio/fat16.h>
#include <global.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <utils/kstrcmp.h>
#include <wex/stwex.h>
#include <wex/testexec.h>
void sh() {
    while (1) {
        char cmd[16];
        kprintf("\nSH> ");
        kgetstr(cmd, sizeof(cmd));
        kflush();
        if (kstrcmp(cmd, "cmp") == 0) {
            kprintf("\nSTR1>");
            char str1[256];
            kgetstr(str1, sizeof(str1));
            kflush();
            kprintf("\nSTR2>");
            char str2[256];
            kgetstr(str2, sizeof(str2));
            kflush();
            if (kstrcmp(str1, str2) == 0) {
                kprintf("\nEqual");
            } else {
                kprintf("\nNot Equal");
            }
        } else if (kstrcmp(cmd, "shutdown") == 0) {
#ifdef VMQEMU
#include <qemu/shutdown.h>
            qemu_shutdown();
#endif
        } else if (kstrcmp(cmd, "help") == 0) {
            kprintf("\nshutdown - shutdown system");
            kprintf("\nhelp - list command");
            kprintf("\ncmp - compare strings");
            kprintf("\nclear - clear screen");
            kprintf("\ntst - Test a WEX Executable");
            kprintf("\nls - List Files in Mounted FAT16");
            kprintf("\nread - Read File Contents from Cluster (512 Bytes)");
            kprintf("\nuser - List Username");
        } else if (kstrcmp(cmd, "tst") == 0) {
            runwex(execr());
        } else if (kstrcmp(cmd, "clear") == 0) {
            kcfp();
        } else if (kstrcmp(cmd, "ls") == 0) {
            fat16_ls();
        } else if (kstrcmp(cmd, "read") == 0) {
            fileconts();
        } else if (kstrcmp(cmd, "user") == 0) {
            kputchar('\n');
            kprintf(name);
        }
    }
}