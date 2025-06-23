#include <KShell/shell.h>
#include <io/kio.h>

void help() {
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
}