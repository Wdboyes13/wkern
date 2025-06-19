#include <KShell/shell.h>
#include <fileio/fileio.h>
#include <global.h>
#include <io/kio.h>
#include <utils/util.h>
#include <wex/stdwex.h>

void sh() {
    while (1) {
        char cmd[16];
        kprintf("\nSH> ");
        kgetstr(cmd, sizeof(cmd));
        kflush();
        if (kstrcmp(cmd, "cmp") == 0) {
            kcmp();
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
        } else if (kstrcmp(cmd, "mkfile") == 0) {
            char filename[9];
            char filext[4];
            kprintf("Enter Filename 7 Letters Max (Without Extension): ");
            kgetstr(filename, 8);
            kflush();
            kprintf("Enter File Extension: ");
            kgetstr(filext, 3);
            kflush();
            mkfile(filename, filext);
        } else if (kstrcmp(cmd, "rm") == 0) {
            char filename[9];
            char filext[4];
            kprintf("Enter Filename 7 Letters Max (Without Extension): ");
            kgetstr(filename, 9);
            kflush();
            kprintf("Enter File Extension: ");
            kgetstr(filext, 4);
            kflush();
            fat16_remove_file(filename, filext);
        }
    }
}