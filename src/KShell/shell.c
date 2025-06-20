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
            kshcmp();
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
            kprintf("\nmkfile - Make a new file");
            kprintf("\nrm - Remove a file");
            kprintf("\nsetname - Set Username");
            kprintf("\nwrite - Write to a File");
        } else if (kstrcmp(cmd, "tst") == 0) {
            runwex(execr());
        } else if (kstrcmp(cmd, "clear") == 0) {
            kcfp();
        } else if (kstrcmp(cmd, "ls") == 0) {
            fat16_ls();
        } else if (kstrcmp(cmd, "read") == 0) {
            char filename[9];
            char filext[4];
            kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
            kgetstr(filename, 9);
            kflush();
            kputchar('\n');
            kprintf("Enter File Extension: ");
            kgetstr(filext, 4);
            kflush();
            kputchar('\n');
            fileconts(filename, filext);
        } else if (kstrcmp(cmd, "user") == 0) {
            kputchar('\n');
            kprintf(name);
        } else if (kstrcmp(cmd, "mkfile") == 0) {
            char filename[9];
            char filext[4];
            kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
            kgetstr(filename, 9);
            kflush();
            kputchar('\n');
            kprintf("Enter File Extension: ");
            kgetstr(filext, 4);
            kflush();
            kputchar('\n');
            mkfile(filename, filext);
        } else if (kstrcmp(cmd, "rm") == 0) {
            char filename[9];
            char filext[4];
            kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
            kgetstr(filename, 9);
            kflush();
            kputchar('\n');
            kprintf("Enter File Extension: ");
            kgetstr(filext, 4);
            kflush();
            kputchar('\n');
            fat16_remove_file(filename, filext);
        } else if (kstrcmp(cmd, "setname") == 0) {
            kprintf("Enter your name: ");
            kgetstr(name, 19);
            kputchar('\n');
        } else if (kstrcmp(cmd, "write") == 0) {
            char filename[9];
            char ext[4];
            char data[510];

            kprintf("\nEnter Filename (Without Extension): ");
            kgetstr(filename, 9);
            kflush();
            kputchar('\n');

            kprintf("Enter File Extension: ");
            kgetstr(ext, 4);
            kflush();
            kputchar('\n');

            kprintf("Start Writing Data");
            kputchar('\n');
            kgetstr(data, 500);
            kflush();

            int len = 0;
            while (len < 510 && data[len] != '\0') {
                len++;
            }
            if (len + 2 < 510) {
                data[len] = '\r';     // Carriage Return
                data[len + 1] = '\n'; // Line Feed
                data[len + 2] =
                    '\0'; // Null terminate (optional, if your strings use it)
            }

            writefile(filename, ext, data, sizeof(data));
        } else {
            kprintf("\nUnknown Command - Try `help`\n");
        }
    }
}