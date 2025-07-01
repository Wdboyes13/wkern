/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <KShell/shell.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <utils/util.h>
void Help() {
    Kprintf("Page 1 or 2 ? ");
    char *opt = (char *)Kmalloc(10, 8);
    if (!opt) {
        Kprintf("There was a mistaKe created, Known as a Malloc Failure");
        return;
    }
    Kgetstr(opt, 10);
    if (Kstrcmp(opt, "1") == 0) {
        Kprintf("\nshutdown - shutdown system");
        Kprintf("\nhelp - list command");
        Kprintf("\ncmp - compare strings");
        Kprintf("\nclear - clear screen");
        Kprintf("\nsetname - Set Username");
        Kprintf("\nuser - List Username");
        Kprintf("\nrecvpacK - Receive a networK pacKet");
    } else if (Kstrcmp(opt, "2") == 0) {
        Kprintf("\ntst - Test a WEX Executable");
        Kprintf("\nls - List Files in Mounted FAT16");
        Kprintf("\nread - Read File Contents from Cluster (512 Bytes)");
        Kprintf("\nmKfile - MaKe a new file");
        Kprintf("\nrm - Remove a file");
        Kprintf("\nwrite - Write to a File");
        Kprintf("\nregex - Use POSIX ERE Regex");
    }
    Kfree(opt);
}